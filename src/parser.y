%code requires { #include <ast.h> }
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <print_ast.h>
#include <ast.h>
    int yylex(void);
    int yyerror(struct var_type_stack** vts, Expr** res_expr, const char* s);

var_type_stack* global_save;
    %}
%union 
 {
     int ival2;
     char* sval;
     level_t ival;
     Expr* expr_1;
     var_type_stack* vstack;
}
%token T_LPAREN T_RPAREN T_COLON T_COMMA T_DOT T_LAMBDA T_FORALL T_EXISTS
%token T_LBRACK T_RBRACK T_TYPE T_FST T_SND T_EQ T_REFL T_J T_W T_TREE
%token T_REC T_IND0 T_IND1 T_IND2 T_BOTTOM T_ONE T_BOOL T_UNIT T_FALSE T_TRUE
%token <sval> IDENT
%token <ival> NUMBER
%token APP_REC
%parse-param { var_type_stack** vts }
%parse-param { Expr** res_expr }

%destructor { free($$); } <sval>

%nonassoc TC
%nonassoc T_LPAREN T_RPAREN T_LBRACK T_RBRACK
%left T_COMMA
%right T_COLON
%right T_DOT
%right T_LAMBDA T_FORALL T_EXISTS
%nonassoc T_EQ T_REFL T_J T_W T_TREE T_REC T_IND0 T_IND1 T_IND2
%left T_BOTTOM T_ONE
%left T_BOOL T_UNIT
%left T_FALSE T_TRUE
%left T_TYPE
%left T_FST T_SND
%define parse.error verbose
%start input

%type <expr_1> input expr atom lambda var_freevar parens true false unit bool one bottom ind2 ind1 ind0 rec tree W J refl eq snd fst type_u exists forall pair
%type <ival2> ps_binders_1 ps_binders lambda_binders idents
%type <vstack> save

%%
input: 
expr { *res_expr = $1; };

expr: expr atom { $$ = make_app($1, $2, false); } | atom;

atom: var_freevar | false | true | bool | one | bottom | unit | type_u | fst | snd | eq | refl | J
| W | tree | rec | ind0 | ind1 | ind2 | exists | forall | lambda | pair | parens;

eq: T_EQ atom atom atom { $$ = make_id($2, $3, $4, false); } ;
refl: T_REFL atom atom { $$ = make_refl($2, $3, false); };
J: T_J atom atom atom atom { $$ = make_j($2, $3, $4, $5, false); } ;
tree: T_TREE atom atom { $$ = make_tree($2, $3, false); };
rec: T_REC atom atom atom { $$ = make_rec($2, $3, $4, false); };
ind0:T_IND0 atom atom { $$ = make_ind0($2, $3, false); };
ind1: T_IND1 atom atom atom { $$ = make_ind1($2, $3, $4, false);};
ind2: T_IND2 atom atom atom atom { $$ = make_ind2($2, $3, $4, $5, false);};
bottom: T_BOTTOM { $$ = make_zero();};
one: T_ONE { $$ = make_one(); };
bool: T_BOOL { $$ = make_two();} ;
unit: T_UNIT { $$ = make_unit(); };
false: T_FALSE {$$ = make_false();};
true: T_TRUE{$$ = make_true();};
type_u: T_TYPE NUMBER { $$ = make_type($2); } ;
fst: T_FST atom { $$ = make_fst($2, false); } ;
snd: T_SND atom { $$ = make_snd($2, false); };

W:
T_W save T_LPAREN ps_binders_1 T_RPAREN T_COMMA expr %prec TC
{
    var_type_stack* initial_vts = $2;
    Expr* w = make_w(copy_expr((*vts)->type), $7, false);
    free_var_stack_until(*vts, initial_vts);
    *vts = initial_vts;
    $$ = w;
}
;

exists:
T_EXISTS save ps_binders T_COMMA expr %prec TC
{
    var_type_stack* initial_vts = $2;
    var_type_stack* vts_iter = *vts;
    Expr* family = $5;
    Expr* f_ty;
    for (int i = 0; i < $3; i++)
    {
        f_ty = vts_iter->type;
        family = make_sig(copy_expr(f_ty), family, false);
        vts_iter = vts_iter->next;
    }
    free_var_stack_until(*vts, initial_vts);
    *vts = initial_vts;
    $$ = family;
}
;

forall:
T_FORALL save ps_binders T_COMMA expr %prec TC
{
    var_type_stack* initial_vts = $2;
    var_type_stack* vts_iter = *vts;
    Expr* cod = $5;
    Expr* dom;
    for (int i = 0; i < $3; i++)
    {
        dom = vts_iter->type;
        cod = make_pi(copy_expr(dom), cod, false);
        vts_iter = vts_iter->next;
    }
    free_var_stack_until(*vts, initial_vts);
    *vts = initial_vts;
    $$ = cod;
}
;

lambda:
T_LAMBDA save lambda_binders T_DOT expr T_COLON expr
{
    var_type_stack* initial_vts = $2;
    var_type_stack* vts_iter = *vts;
    Expr* term = $5;
    Expr* annot = $7;
    int count = $3;
    for (int i = 0; i < count; i++)
    {
        term = make_lam(NULL, term, false);
        vts_iter = vts_iter->next;
    }
    Expr* term_iter = term;
    while (count--)
    {
        term_iter->lam.type_annot = annot;
        term_iter = term_iter->lam.expr;
        if (count) annot = copy_expr(next_pi(annot));
    }
    free_var_stack_until(*vts, initial_vts);
    *vts = initial_vts;
    $$ = term;
}
;

pair:
T_LBRACK expr T_COMMA expr T_RBRACK T_COLON expr
{
    Expr* elem_1 = $2;
    Expr* elem_2 = $4;
    Expr* annot = $7;
    $$ = make_pair(annot, elem_1, elem_2, false);
}
;

parens:
T_LPAREN expr T_RPAREN
{
    $$ = $2;
}
;

var_freevar:
IDENT
{
    debruijn ind;
    char* name = $1;
    if (!get_debruijn(*vts, name, &ind))
    {
        Expr* def= make_def(name, false);
        $$ = def;
    }
    else
    {
        Expr* var = make_var(ind, name, false);
        $$ = var;
    }
}
;

save:
{
    $$ = *vts;
}
;

lambda_binders:
IDENT
{
    *vts = add_var_stack(*vts, $1, NULL);
    $$ = 1;
}
| IDENT lambda_binders
{
    *vts = add_var_stack(*vts, $1, NULL);
    $$ = 1 + $2;
}
;

ps_binders:
T_LPAREN ps_binders_1 T_RPAREN
{
    $$ = $2;
}
| T_LPAREN ps_binders_1 T_RPAREN ps_binders
{
    $$ = $2 + $4;
}
;

push1:
{
    global_save = *vts;
}
pop1:
{
    *vts = global_save;
}

ps_binders_1:
push1 idents save pop1 T_COLON expr
{
    *vts = $3;
    var_type_stack* vts_iter = *vts;
    int count = $2;
    while (count--)
    {
        if (!count) vts_iter->type = $6;
        else vts_iter->type = copy_expr($6);
	if ((vts_iter->type)->tag == VAR)
	{
	    (vts_iter->type)->var.index += count;
	}
        vts_iter = vts_iter->next;
    }
    $$ = $2;
}
;

idents:
IDENT
{
    *vts = add_var_stack(*vts, $1, NULL);
    $$ = 1;
}
| idents IDENT
{
    *vts = add_var_stack(*vts, $2, NULL);
    $$ = $1 + 1;
}
;

%%
int yyerror(struct var_type_stack** vts, Expr** res_expr, const char* s)
{
    printf("error: %s\n", s);
    return -1;
}
