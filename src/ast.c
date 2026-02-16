#include <ast.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* strdup(const char* s)
{
  size_t len = strlen(s) + 1;
  void* new = malloc(len);
  if (new == NULL)
    return NULL;
  return (char*)memcpy(new, s, len);
}
/*

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
*/

bool cmp_expr(Expr* expr1, Expr* expr2)
{
  if (GET_TAG(expr1) != GET_TAG(expr2))
  {
    return false;
  }
  if (GET_TAG(expr1) > TYPE && GET_TAG(expr1) <= TRUE_T)
  {
    return true;
  }
  if (GET_TAG(expr1) > TRUE_T)
  {
    uint8_t args = GET_TAG(expr1) >> 5;
    bool cond1 = cmp_expr(GET_EXPR1(expr1), GET_EXPR1(expr2));
    bool cond2, cond3, cond4, cond5;
    if (args > 1)
    {
      cond2 = cond1 && cmp_expr(GET_EXPR2(expr1), GET_EXPR2(expr2)); 
    }
    else
    {
      return cond1;
    }
    if (args > 2)
    {
      cond3 = cond2 && cmp_expr(GET_EXPR3(expr1), GET_EXPR3(expr2)); 
    }
    else
    {
      return cond2;
    }
    if (args > 3)
    {
      cond4 = cond3 && cmp_expr(GET_EXPR4(expr1), GET_EXPR4(expr2)); 
    }
    else
    {
      return cond3;
    }
    if (args > 4)
    {
      cond5 = cond4 && cmp_expr(GET_EXPR5(expr1), GET_EXPR5(expr2)); 
      return cond5;
    }
    else
    {
      return cond4;
    }
  }
  if (GET_TAG(expr1) == VAR)
  {
    return (GET_INDEX(expr1) == GET_INDEX(expr2));
  }
  if (GET_TAG(expr1) == DEF)
  {
    return (!strcmp(GET_NAME(expr1), GET_NAME(expr2)));
  }
  if (GET_TAG(expr1) == TYPE)
  {
    return (GET_LEVEL(expr1) == GET_LEVEL(expr2));
  }
  return false;
}

Expr* copy_expr(Expr* src)
{
  Expr* dst;
  if (src == NULL)
    return NULL;
  if (GET_TAG(src) == VAR)
  {
    dst = malloc(sizeof(uint8_t) + sizeof(Var_data));
    CAST_NAME(dst)->tag = VAR;
    CAST_NAME(dst)->var.name = strdup(CAST_NAME(src)->var.name);
    CAST_NAME(dst)->var.index = CAST_NAME(src)->var.index;
    return dst;
  }
  else if (GET_TAG(src) == DEF)
  {
    dst = malloc(sizeof(uint8_t) + sizeof(Def_data));
    CAST_NAME(dst)->tag = DEF;
    CAST_NAME(dst)->def.name = strdup(CAST_NAME(src)->def.name);
    return dst;
  }
  else if (GET_TAG(src) == TYPE)
  {
    dst = malloc(sizeof(uint8_t) + sizeof(Type_data));
    CAST_NAME(dst)->tag = TYPE;
    CAST_NAME(dst)->type.level = CAST_NAME(src)->type.level;
    return dst;
  }
  else if (GET_TAG(src) <= TRUE_T)
  {
    dst = malloc(sizeof(uint8_t));
    CAST_NAME(dst)->tag = GET_TAG(src);
    return dst;
  }
  else
  {
    uint8_t args = GET_TAG(src) >> 5;
    dst = malloc(sizeof(uint8_t) + sizeof(Expr*)*args);
    GET_TAG(dst) = GET_TAG(src);
    GET_EXPR1(dst) = copy_expr(GET_EXPR1(src));
    if (args > 1)
    {
      GET_EXPR2(dst) = copy_expr(GET_EXPR2(src));
    }
    else
    {
      return dst;
    }
    if (args > 2)
    {
      GET_EXPR3(dst) = copy_expr(GET_EXPR3(src));
    }
    else
    {
      return dst;
    }
    if (args > 3)
    {
      GET_EXPR4(dst) = copy_expr(GET_EXPR4(src));
    }
    else
    {
      return dst;
    }
    if (args > 4)
    {
      GET_EXPR5(dst) = copy_expr(GET_EXPR5(src));
      return dst;
    }
    else
    {
      return dst;
    }
  }
}

Expr* make_expr_0(tag_t tag)
{
  Expr* node = malloc(sizeof(uint8_t));
  GET_TAG(node) = tag;
  return node;
}

Expr* make_expr_1(tag_t tag, Expr* arg1, bool copy) // not appliable to def, var, type
{
  if (copy)
  {
    arg1 = copy_expr(arg1);
  }

  Expr* node = malloc(sizeof(uint8_t) + sizeof(Expr*)*1);
  GET_TAG(node) = tag;
  GET_EXPR1(node) = arg1;
  return node;
}

Expr* make_expr_2(tag_t tag, Expr* arg1, Expr* arg2, bool copy)
{
  if (copy)
  {
    arg1 = copy_expr(arg1);
    arg2 = copy_expr(arg2);
  }

  Expr* node = malloc(sizeof(uint8_t) + sizeof(Expr*)*2);

  GET_TAG(node) = tag;
  GET_EXPR1(node) = arg1;
  GET_EXPR2(node) = arg2;
  
  return node;
}

Expr* make_expr_3(tag_t tag, Expr* arg1, Expr* arg2, Expr* arg3, bool copy)
{
  if (copy)
  {
    arg1 = copy_expr(arg1);
    arg2 = copy_expr(arg2);
    arg3 = copy_expr(arg3);
  }

  Expr* node = malloc(sizeof(uint8_t) + sizeof(Expr*)*3);
  GET_TAG(node) = tag;
  GET_EXPR1(node) = arg1;
  GET_EXPR2(node) = arg2;
  GET_EXPR3(node) = arg3;
  return node;
}

Expr* make_expr_4(tag_t tag, Expr* arg1, Expr* arg2, Expr* arg3, Expr* arg4,
                  bool copy)
{
  if (copy)
  {
    arg1 = copy_expr(arg1);
    arg2 = copy_expr(arg2);
    arg3 = copy_expr(arg3);
    arg4 = copy_expr(arg4);
  }

  Expr* node = malloc(sizeof(uint8_t) + sizeof(Expr*)*4);
  GET_TAG(node) = tag;
  GET_EXPR1(node) = arg1;
  GET_EXPR2(node) = arg2;
  GET_EXPR3(node) = arg3;
  GET_EXPR4(node) = arg4;
  return node;
}

Expr* make_expr_5(tag_t tag, Expr* arg1, Expr* arg2, Expr* arg3, Expr* arg4, Expr* arg5,
                  bool copy)
{
  if (copy)
  {
    arg1 = copy_expr(arg1);
    arg2 = copy_expr(arg2);
    arg3 = copy_expr(arg3);
    arg4 = copy_expr(arg4);
    arg5 = copy_expr(arg5);
  }

  Expr* node = malloc(sizeof(uint8_t) + sizeof(Expr*)*5);
  
  GET_TAG(node) = tag;
  GET_EXPR1(node) = arg1;
  GET_EXPR2(node) = arg2;
  GET_EXPR3(node) = arg3;
  GET_EXPR4(node) = arg4;
  GET_EXPR5(node) = arg5;
  return node;
}

Expr* make_type(level_t level)
{
  Expr* node = malloc(sizeof(uint8_t) + sizeof(Type_data));

  GET_TAG(node) = TYPE;
  GET_LEVEL(node) = level;
  return node;
}
Expr* make_def(char* name, bool copy)
{
  Expr* node = malloc(sizeof(uint8_t) + sizeof(Def_data));
  GET_TAG(node) = DEF;
  if (copy)
  {
    GET_NAME(node) = strdup(name);
  }
  else
  {
    GET_NAME(node) = name;
  }
  return node;
}

Expr* make_var(debruijn id, char* name, bool copy)
{
  Expr* node = malloc(sizeof(uint8_t) + sizeof(Var_data));
  GET_TAG(node) = VAR;
  GET_INDEX(node) = id;
  if (copy)
  {
    GET_NAME(node) = strdup(name);
  }
  else
  {
    GET_NAME(node) = name;
  }
  return node;
}

void free_expr(Expr* expr)
{
  if (!expr)
    return;
  if (GET_TAG(expr) < 2) // VAR and DEF
  {
    free(GET_NAME(expr));
    free(expr);
    return;
  }
  if (GET_TAG(expr) <= TRUE_T)
  {
    free(expr);
    return;
  }
  uint8_t args = GET_TAG(expr) >> 5;
  free_expr(GET_EXPR1(expr));
  if (args > 1)
  {
   free_expr(GET_EXPR2(expr));
  }
  else goto return_free_expr;
  if (args > 2)
  {
    free_expr(GET_EXPR3(expr));
  }
  else goto return_free_expr;
  if (args > 3)
  {
   free_expr(GET_EXPR4(expr));
  }
  else goto return_free_expr;
  if (args > 4)
  {
    free_expr(GET_EXPR5(expr));
  }
return_free_expr:
  free(expr);
  return;
}


void shift(Expr* expr, int d, int cutoff)
{
  if (!expr)
    return;
  if (GET_TAG(expr) == VAR)
  {
    if (CAST_NAME(expr)->var.index >= (debruijn)cutoff)
    {
      CAST_NAME(expr)->var.index += d;
    }
  }
  else if (GET_TAG(expr) <= TRUE_T)
  {
    return;
  }
  else if (GET_TAG(expr) == APP)
  {
    shift(CAST_NAME(expr)->app.left, d, cutoff);
    shift(CAST_NAME(expr)->app.right, d, cutoff);
  }
  else if (CAST_NAME(expr)->tag == LAM)
  {
    shift(CAST_NAME(expr)->lam.type_annot, d, cutoff);
    shift(CAST_NAME(expr)->lam.expr, d, cutoff + 1);
  }
  else if (CAST_NAME(expr)->tag == PI)
  {
    shift(CAST_NAME(expr)->pi.dom, d, cutoff);
    shift(CAST_NAME(expr)->pi.cod, d, cutoff + 1);
  }
  else if (CAST_NAME(expr)->tag == SIG)
  {
    shift(CAST_NAME(expr)->sig.f_ty, d, cutoff);
    shift(CAST_NAME(expr)->sig.family, d, cutoff + 1);
  }
  else if (CAST_NAME(expr)->tag == PAIR)
  {
    shift(CAST_NAME(expr)->pair.type_annot, d, cutoff);
    shift(CAST_NAME(expr)->pair.a, d, cutoff);
    shift(CAST_NAME(expr)->pair.b, d, cutoff);
  }
  else if (CAST_NAME(expr)->tag == FST)
  {
    shift(CAST_NAME(expr)->fst.expr, d, cutoff);
  }
  else if (CAST_NAME(expr)->tag == SND)
  {
    shift(CAST_NAME(expr)->snd.expr, d, cutoff);
  }
  else if (CAST_NAME(expr)->tag == ID)
  {
    shift(CAST_NAME(expr)->id.ty, d, cutoff);
    shift(CAST_NAME(expr)->id.a, d, cutoff);
    shift(CAST_NAME(expr)->id.b, d, cutoff);
  }
  else if (CAST_NAME(expr)->tag == REFL)
  {
    shift(CAST_NAME(expr)->refl.ty, d, cutoff);
    shift(CAST_NAME(expr)->refl.a, d, cutoff);
  }
  else if (CAST_NAME(expr)->tag == J)
  {
    shift(CAST_NAME(expr)->j.C, d, cutoff);
    shift(CAST_NAME(expr)->j.c, d, cutoff);
    shift(CAST_NAME(expr)->j.a, d, cutoff);
    shift(CAST_NAME(expr)->j.b, d, cutoff);
    shift(CAST_NAME(expr)->j.p, d, cutoff);
  }
  else if (CAST_NAME(expr)->tag == W)
  {
    shift(CAST_NAME(expr)->w.C, d, cutoff);
    shift(CAST_NAME(expr)->w.family, d, cutoff + 1);
  }
  else if (CAST_NAME(expr)->tag == TREE)
  {
    shift(CAST_NAME(expr)->tree.root, d, cutoff);
    shift(CAST_NAME(expr)->tree.subtr, d, cutoff);
  }
  else if (CAST_NAME(expr)->tag == REC)
  {
    shift(CAST_NAME(expr)->rec.D, d, cutoff);
    shift(CAST_NAME(expr)->rec.tree_D, d, cutoff);
    shift(CAST_NAME(expr)->rec.t, d, cutoff);
  }
  else if (CAST_NAME(expr)->tag == IND0)
  {
    shift(CAST_NAME(expr)->ind0.C, d, cutoff);
    shift(CAST_NAME(expr)->ind0.x, d, cutoff);
  }
  else if (CAST_NAME(expr)->tag == IND1)
  {
    shift(CAST_NAME(expr)->ind1.C, d, cutoff);
    shift(CAST_NAME(expr)->ind1.x, d, cutoff);
    shift(CAST_NAME(expr)->ind1.y, d, cutoff);
  }
  else if (CAST_NAME(expr)->tag == IND2)
  {
    shift(CAST_NAME(expr)->ind2.C, d, cutoff);
    shift(CAST_NAME(expr)->ind2.x, d, cutoff);
    shift(CAST_NAME(expr)->ind2.y, d, cutoff);
    shift(CAST_NAME(expr)->ind2.z, d, cutoff);
  }
}

Expr* shift_expr(Expr* expr, int d, int cutoff)
{
    Expr* c = copy_expr(expr);
    shift(c, d, cutoff);
    return c;
}

Expr* subst_1(Expr* expr, debruijn n, Expr* r, debruijn acc)
{
  if (!expr)
    return NULL;
  if (CAST_NAME(expr)->tag == VAR)
  {
    if (CAST_NAME(expr)->var.index == n+acc)
    {
      return shift_expr(r, acc, 0);
    }
    else if (CAST_NAME(expr)->var.index > n+acc)
    {
      // todo: possible to optimize
      return copy_expr(expr); 
    }
    else
    {
      return copy_expr(expr);
    }
  }
  else if (CAST_NAME(expr)->tag <= TRUE_T)
  {
    return copy_expr(expr);
  }
  else if (CAST_NAME(expr)->tag == APP)
  {
    return make_app(subst_1(CAST_NAME(expr)->app.left,n,r,acc), subst_1(CAST_NAME(expr)->app.right,n,r, acc), false);
  }
  else if (CAST_NAME(expr)->tag == LAM)
  {
    Expr* type_annot = subst_1(CAST_NAME(expr)->lam.type_annot, n, r, acc);
    Expr* new_expr = subst_1(CAST_NAME(expr)->lam.expr, n, r, acc+1);
    return make_lam(type_annot, new_expr, false);
  }
  else if (CAST_NAME(expr)->tag == PI)
  {
    Expr* dom = subst_1(CAST_NAME(expr)->pi.dom, n, r, acc);
    Expr* cod = subst_1(CAST_NAME(expr)->pi.cod, n, r, acc+1);
    return make_pi(dom, cod, false);
  }
  else if (CAST_NAME(expr)->tag == SIG)
  {
    Expr* f_ty = subst_1(CAST_NAME(expr)->sig.f_ty, n, r, acc);
    Expr* family = subst_1(CAST_NAME(expr)->sig.family, n, r, acc+1);
    return make_sig(f_ty, family, false);
  }
  else if (CAST_NAME(expr)->tag == PAIR)
  {
    Expr* type_annot = subst_1(CAST_NAME(expr)->pair.type_annot, n, r, acc);
    Expr* a = subst_1(CAST_NAME(expr)->pair.a, n, r, acc);
    Expr* b = subst_1(CAST_NAME(expr)->pair.b, n, r, acc);
    return make_pair(type_annot, a, b, false);
  }
  else if (CAST_NAME(expr)->tag == FST)
  {
    return make_fst(subst_1(CAST_NAME(expr)->fst.expr,n,r, acc), false);
  }
  else if (CAST_NAME(expr)->tag == SND)
  {
    return make_snd(subst_1(CAST_NAME(expr)->snd.expr,n,r, acc), false);
  }
  else if (CAST_NAME(expr)->tag == ID)
  {
    Expr* ty = subst_1(CAST_NAME(expr)->id.ty, n, r, acc);
    Expr* a = subst_1(CAST_NAME(expr)->id.a, n, r, acc);
    Expr* b = subst_1(CAST_NAME(expr)->id.b, n, r, acc);
    return make_id(ty, a, b, false);
  }
  else if (CAST_NAME(expr)->tag == REFL)
  {
    Expr* ty = subst_1(CAST_NAME(expr)->refl.ty, n, r, acc);
    Expr* a = subst_1(CAST_NAME(expr)->refl.a, n, r, acc);
    return make_refl(ty, a, false);
  }
  else if (CAST_NAME(expr)->tag == J)
  {
    Expr* C = subst_1(CAST_NAME(expr)->j.C, n, r, acc);
    Expr* c = subst_1(CAST_NAME(expr)->j.c, n, r, acc);
    Expr* a = subst_1(CAST_NAME(expr)->j.a, n, r, acc);
    Expr* b = subst_1(CAST_NAME(expr)->j.b, n, r, acc);
    Expr* p = subst_1(CAST_NAME(expr)->j.p, n, r, acc);
    return make_j(C, c, a, b, p, false);
  }
  else if (CAST_NAME(expr)->tag == W)
  {
    Expr* C = subst_1(CAST_NAME(expr)->w.C, n, r, acc);
    Expr* A = subst_1(CAST_NAME(expr)->w.family, n, r, acc+1);
    return make_w(C, A, false);
  }
  else if (CAST_NAME(expr)->tag == TREE)
  {
    Expr* root = subst_1(CAST_NAME(expr)->tree.root, n, r, acc);
    Expr* subtr = subst_1(CAST_NAME(expr)->tree.subtr, n, r, acc);
    return make_tree(root, subtr, false);
  }
  else if (CAST_NAME(expr)->tag == REC)
  {
    Expr* D = subst_1(CAST_NAME(expr)->rec.D, n, r, acc);
    Expr* tree_D = subst_1(CAST_NAME(expr)->rec.tree_D, n, r, acc);
    Expr* t = subst_1(CAST_NAME(expr)->rec.t, n, r, acc);
    return make_rec(D, tree_D, t, false);
  }
  else if (CAST_NAME(expr)->tag == IND0)
  {
    Expr* C = subst_1(CAST_NAME(expr)->ind0.C, n, r, acc);
    Expr* x = subst_1(CAST_NAME(expr)->ind0.x, n, r, acc);
    return make_ind0(C, x, false);
  }
  else if (CAST_NAME(expr)->tag == IND1)
  {
    Expr* C = subst_1(CAST_NAME(expr)->ind1.C, n, r, acc);
    Expr* x = subst_1(CAST_NAME(expr)->ind1.x, n, r, acc);
    Expr* y = subst_1(CAST_NAME(expr)->ind1.y, n, r, acc);
    return make_ind1(C, x, y, false);
  }
  else if (CAST_NAME(expr)->tag == IND2)
  {
    Expr* C = subst_1(CAST_NAME(expr)->ind2.C, n, r, acc);
    Expr* x = subst_1(CAST_NAME(expr)->ind2.x, n, r, acc);
    Expr* y = subst_1(CAST_NAME(expr)->ind2.y, n, r, acc);
    Expr* z = subst_1(CAST_NAME(expr)->ind2.z, n, r, acc);
    return make_ind2(C, x, y, z, false);
  }
  return NULL;
}

Expr* subst(Expr* expr, debruijn n, Expr* r)
{
  return subst_1(expr, n, r, 0);
}

Expr* subst_top(Expr* expr, Expr* r)
{
  Expr* cp_expr = copy_expr(expr);
  Expr* shift_r = shift_expr(r, 1, 0);
  Expr* subst_sr = subst(cp_expr, 0, shift_r);
  shift(subst_sr, -1, 0);
  free_expr(shift_r);
  free_expr(cp_expr);
  return subst_sr;
}
