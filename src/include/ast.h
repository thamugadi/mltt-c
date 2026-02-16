#pragma once
#include <common.h>
#include <stdbool.h>
#include <stdlib.h>
typedef uint16_t level_t;
typedef uint32_t debruijn;
typedef uint8_t tag_t;

// constructor >> 5 is the number of arguments that it takes

enum
{
  DEF,
  VAR,
  TYPE,
  ZERO, ONE, UNIT, TWO, FALSE_T, TRUE_T, 
  APP = 0b01000000, 
  LAM = 0b01000001, 
  PI  = 0b01000010, 
  SIG = 0b01000011, 
  PAIR= 0b01100100, 
  FST = 0b00100101, 
  SND = 0b00100110,
  ID  = 0b01100111,
  REFL= 0b01001000,
  J   = 0b10101001, 
  W   = 0b01001010,
  TREE= 0b01001011,
  REC = 0b01101100,
  IND0= 0b01001101,
  IND1= 0b01101110,
  IND2= 0b10001111, 
};

typedef void Expr;

union field_1
{
  char* name;
  level_t level;
  Expr* expr_1;
};

union field_2
{
  debruijn index;
  Expr* expr_2;
};

// everything pointer to one of the datatypes below are casted with this generic one
// to access fields in generic cases. I'm not sure if this is UB or not

typedef struct __attribute__((packed)) Expr_generic
{
  tag_t tag;
  union field_1 f1;
  union field_2 f2;
  Expr* expr_3;
  Expr* expr_4;
  Expr* expr_5;
} Expr_generic;

#define GET_TAG(ex) ((Expr_generic*)ex)->tag
#define GET_NAME(ex) ((Expr_generic*)ex)->f1.name
#define GET_INDEX(ex) ((Expr_generic*)ex)->f2.index
#define GET_LEVEL(ex) ((Expr_generic*)ex)->f1.level

#define GET_EXPR1(ex) ((Expr_generic*)ex)->f1.expr_1
#define GET_EXPR2(ex) ((Expr_generic*)ex)->f2.expr_2
#define GET_EXPR3(ex) ((Expr_generic*)ex)->expr_3
#define GET_EXPR4(ex) ((Expr_generic*)ex)->expr_4
#define GET_EXPR5(ex) ((Expr_generic*)ex)->expr_5

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
  Expr* C;
  Expr* c;
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

typedef struct __attribute__((packed)) Expr_named 
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
} Expr_named;

#define CAST_NAME(ex) ((Expr_named*)ex)

Expr* make_expr_0(tag_t tag);
Expr* make_expr_1(tag_t tag, Expr* arg1, bool copy);
Expr* make_expr_2(tag_t tag, Expr* arg1, Expr* arg2, bool copy);
Expr* make_expr_3(tag_t tag, Expr* arg1, Expr* arg2, Expr* arg3, bool copy);
Expr* make_expr_4(tag_t tag, Expr* arg1, Expr* arg2, Expr* arg3, Expr* arg4, bool copy);
Expr* make_expr_5(tag_t tag, Expr* arg1, Expr* arg2, Expr* arg3, Expr* arg4, Expr* arg5, bool copy);

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

static inline Expr* make_j(Expr* C, Expr* c, Expr* a, Expr* b, Expr* p, bool copy)
{
  return make_expr_5(J, C, c, a, b, p, copy);
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

char* strdup (const char *s);

void shift(Expr* expr, int d, int cutoff);
Expr* shift_expr(Expr* expr, int d, int cutoff);

Expr* subst_top(Expr* expr, Expr* r);
