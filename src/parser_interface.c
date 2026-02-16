#include <var_env.h>
#include <def_env.h>
#include <ast.h>
#include <parser_helper.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <parser.tab.h>
extern char* ARGS[MAX_ARGS];
typedef struct yy_buffer_state* YY_BUFFER_STATE;
extern int yydebug;
YY_BUFFER_STATE yy_scan_string(const char* str);
void yy_delete_buffer(YY_BUFFER_STATE buffer);
int yylex_destroy(void);
int yyparse(var_type_stack** vts, Expr** res_expr);

Expr* parse_expr(char* str, uint32_t arg_count, Expr* ty)
{
  Expr* annot = ty;
  Expr* annot2 = ty;
  Expr* expr = NULL;
  var_type_stack* vts = NULL;
  bool first = true;
  Expr* expr_init = NULL;
  // having arguments in a definition is translated to additional lambda terms
  for (uint32_t i = 0; i < arg_count; i++)
  {
    if (!annot2)
    {
      free_expr(expr);
      free_var_stack(vts);
      fail_msg("todo:msg");
      return NULL;
    }
    expr = make_lam(NULL, expr, false);
    if (first)
    {
      expr_init = expr;
      first = false;
    }
    // idk if it's really necessary to include the term?
    //vts = add_var_stack(vts, strdup(ARGS[i]), copy_expr(annot2->pi.dom));
    vts = add_var_stack(vts, strdup(ARGS[i]), NULL);

    // just to handle errors where there's more arguments than pi-types from the top
    annot2 = next_pi(annot2); 
  }
  Expr* top_expr = expr;
  Expr* expr_iter = expr;
  // wrapping the type in pi-types corresponding to the added lambda abstractions 
  for (uint32_t i = arg_count; i > 0; i--)
  {
    CAST_NAME(expr_iter)->lam.type_annot = copy_expr(annot);
    expr_iter = CAST_NAME(expr_iter)->lam.expr;
    annot = next_pi(annot);
  }
  YY_BUFFER_STATE buffer = yy_scan_string(str);
  Expr* expr2;
  //  yydebug = 1;
  if (yyparse(&vts, &expr2))
  {
    free_var_stack(vts);
    free_expr(expr);
    yy_delete_buffer(buffer);
    yylex_destroy();
    return NULL;
  }
  yy_delete_buffer(buffer);
  free_var_stack(vts);
  yylex_destroy();
  if (arg_count)
  {
    CAST_NAME(expr_init)->lam.expr = expr2;
    return top_expr;
  }
  else
  {
    return expr2;
  }
}
