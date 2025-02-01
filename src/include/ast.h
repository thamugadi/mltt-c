#pragma once
#include <common.h>
#include <stdbool.h>
#include <stdlib.h>
typedef uint16_t level_t;
typedef uint32_t debruijn;
typedef uint8_t tag_t;
enum
{
  DEF,
  VAR,
  TYPE,
  ZERO, ONE, UNIT, TWO, FALSE_T, TRUE_T, 
  APP, 
  LAM, 
  PI, 
  SIG, 
  PAIR, 
  FST, 
  SND,
  ID,
  REFL,
  J, 
  W,
  TREE,
  REC,
  IND0,
  IND1,
  IND2 
};

typedef struct __attribute__((packed)) Expr Expr;

typedef struct __attribute__((packed)) Var_data
{
  char* name;
  debruijn index;
} Var_data;

typedef struct __attribute__((packed)) Def_data
{
  char* name;
} Def_data;

typedef struct __attribute__((packed)) Type_data
{
  level_t level;
} Type_data;

typedef struct __attribute__((packed)) App_data
{
  Expr* left;
  Expr* right;
} App_data;

typedef struct __attribute__((packed)) Pi_data
{
  Expr* dom;
  Expr* cod;
} Pi_data;

typedef struct __attribute__((packed)) Lam_data
{
  Expr* type_annot;
  Expr* expr;
} Lam_data;

typedef struct __attribute__((packed)) Sig_data
{
  Expr* f_ty;
  Expr* family;
} Sig_data;

typedef struct __attribute__((packed)) Pair_data
{
  Expr* type_annot;
  Expr* a;
  Expr* b;
} Pair_data;

typedef struct __attribute__((packed)) Fst_data
{
  Expr* expr;
} Fst_data;

typedef struct __attribute__((packed)) Snd_data
{
  Expr* expr;
} Snd_data;

typedef struct __attribute__((packed)) Id_data
{
  Expr* ty;
  Expr* a;
  Expr* b;
} Id_data;

typedef struct __attribute__((packed)) Refl_data
{
  Expr* ty;
  Expr* a;
} Refl_data;

typedef struct __attribute__((packed)) J_data
{
  Expr* t;
  Expr* a;
  Expr* b;
  Expr* p;
} J_data;

typedef struct __attribute__((packed)) W_data
{
  Expr* C;
  Expr* family;
} W_data;

typedef struct __attribute__((packed)) Tree_data
{
  Expr* root;
  Expr* subtr;
} Tree_data;

typedef struct __attribute__((packed)) Rec_data
{
  Expr* D;
  Expr* tree_D;
  Expr* t;
} Rec_data;

typedef struct __attribute__((packed)) Ind0_data
{
  Expr* C;
  Expr* x;
} Ind0_data;

typedef struct __attribute__((packed)) Ind1_data
{
  Expr* C;
  Expr* x;
  Expr* y;
} Ind1_data;

typedef struct __attribute__((packed)) Ind2_data
{
  Expr* C;
  Expr* x;
  Expr* y;
  Expr* z;
} Ind2_data;

typedef struct __attribute__((packed)) Expr
{
  tag_t tag;
  union __attribute__((packed))
  {
    struct __attribute__((packed)) Var_data var;
    struct __attribute__((packed)) Type_data type;
    struct __attribute__((packed)) Def_data def;
    struct __attribute__((packed)) App_data app;
    struct __attribute__((packed)) Pi_data pi;
    struct __attribute__((packed)) Lam_data lam;
    struct __attribute__((packed)) Sig_data sig;
    struct __attribute__((packed)) Pair_data pair;
    struct __attribute__((packed)) Fst_data fst;
    struct __attribute__((packed)) Snd_data snd;
    struct __attribute__((packed)) Id_data id;
    struct __attribute__((packed)) Refl_data refl;
    struct __attribute__((packed)) J_data j;
    struct __attribute__((packed)) W_data w;
    struct __attribute__((packed)) Tree_data tree;
    struct __attribute__((packed)) Rec_data rec;
    struct __attribute__((packed)) Ind0_data ind0;
    struct __attribute__((packed)) Ind1_data ind1;
    struct __attribute__((packed)) Ind2_data ind2;
  };
} Expr;

Expr* make_expr_0(tag_t tag);
Expr* make_expr_1(tag_t tag, Expr* arg1, bool copy);
Expr* make_expr_2(tag_t tag, Expr* arg1, Expr* arg2, bool copy);
Expr* make_expr_3(tag_t tag, Expr* arg1, Expr* arg2, Expr* arg3, bool copy);
Expr* make_expr_4(tag_t tag, Expr* arg1, Expr* arg2, Expr* arg3, Expr* arg4, bool copy);

Expr* make_var(debruijn debruijn, char* name, bool copy);
Expr* make_def(char* name, bool copy);
Expr* make_type(level_t level);

static inline Expr* make_zero(void)
{
  return make_expr_0(ZERO);
}

static inline Expr* make_one(void)
{
  return make_expr_0(ONE);
}

static inline Expr* make_unit(void)
{
  return make_expr_0(UNIT);
}

static inline Expr* make_two(void)
{
  return make_expr_0(TWO);
}

static inline Expr* make_false(void)
{
  return make_expr_0(FALSE_T);
}

static inline Expr* make_true(void)
{
  return make_expr_0(TRUE_T);
}

static inline Expr* make_fst(Expr* expr, bool copy)
{
  return make_expr_1(FST, expr, copy);
}

static inline Expr* make_snd(Expr* expr, bool copy)
{
  return make_expr_1(SND, expr, copy);
}

static inline Expr* make_app(Expr* left, Expr* right,
                                                     bool copy)
{
  return make_expr_2(APP, left, right, copy);
}

static inline Expr* make_lam(Expr* type_annot, Expr* expr,
                                                     bool copy)
{
  return make_expr_2(LAM, type_annot, expr, copy);
}

static inline Expr* make_pi(Expr* dom, Expr* cod,
                                                    bool copy)
{
  return make_expr_2(PI, dom, cod, copy);
}

static inline Expr* make_sig(Expr* f_ty, Expr* family, 
                                                     bool copy)
{
  return make_expr_2(SIG, f_ty, family, copy);
}

static inline Expr* make_pair(Expr* type_annot, Expr* a, Expr* b,
                                                      bool copy)
{
  return make_expr_3(PAIR, type_annot, a, b, copy);
}

static inline Expr* make_w(Expr* C, Expr* family,
                                                   bool copy)
{
  return make_expr_2(W, C, family, copy);
}

static inline Expr* make_tree(Expr* root, Expr* subtr,
                                                      bool copy)
{
  return make_expr_2(TREE, root, subtr, copy);
}

static inline Expr* make_id(Expr* ty, Expr* a, Expr* b,
                                                    bool copy)
{
  return make_expr_3(ID, ty, a, b, copy);
}

static inline Expr* make_refl(Expr* ty, Expr* a, bool copy)
{
  return make_expr_2(REFL, ty, a, copy);
}

static inline Expr* make_j(Expr* t, Expr* a, Expr* b, Expr* p, bool copy)
{
  return make_expr_4(J, t, a, b, p, copy);
}

static inline Expr* make_rec(Expr* D, Expr* tree_D,
                                                     Expr* t, bool copy)
{
  return make_expr_3(REC, D, tree_D, t, copy);
}

static inline Expr* make_ind0(Expr* C, Expr* x,
                                                      bool copy)
{
  return make_expr_2(IND0, C, x, copy);
}

static inline Expr* make_ind1(Expr* C, Expr* x, Expr* y,
                                                      bool copy)
{
  return make_expr_3(IND1, C, x, y, copy);
}

static inline Expr* make_ind2(Expr* C, Expr* x, Expr* y, Expr* z,
                                                      bool copy)
{
  return make_expr_4(IND2, C, x, y, z, copy);
}
void free_expr(Expr* expr);

bool cmp_expr(Expr* ty1, Expr* ty2);
Expr* subst_expr(Expr* expr, Expr* a, debruijn x);
Expr* copy_expr(Expr* src);

typedef struct var_type_stack
{
  char* name;
  Expr* type;
  struct var_type_stack* next;
} var_type_stack;

void free_var_stack(var_type_stack* vts);
void free_var_stack_until(var_type_stack* vts, var_type_stack* until);
var_type_stack* add_var_stack(var_type_stack* vts, char* s, Expr* ty);
bool get_debruijn(var_type_stack* vts, char* s, debruijn* out);

Expr* next_pi(Expr* expr);

char* strdup (const char *s);

void shift(Expr* expr, int d, int cutoff);
Expr* shift_expr(Expr* expr, int d, int cutoff);
Expr* subst(Expr* expr, debruijn n, Expr* r);

Expr* subst_top(Expr* expr, Expr* r);
