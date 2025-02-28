#include <ast.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// todo: optimize subst

// this strdup definition was copypasted from somewhere
char* strdup(const char* s)
{
  size_t len = strlen(s) + 1;
  void* new = malloc(len);
  if (new == NULL)
    return NULL;
  return (char*)memcpy(new, s, len);
}
bool cmp_expr(Expr* expr1, Expr* expr2)
{
  bool cond1, cond2, cond3, cond4;
  if (expr1->tag != expr2->tag)
  {
    return false;
  }
  if (expr1->tag > TYPE && expr1->tag <= TRUE_T)
  {
    return true;
  }
  if (expr1->tag > TRUE_T)
  {
    cond1 = cmp_expr(expr1->ind2.C, expr2->ind2.C);
    if (expr1->ind2.x)
    {
      cond2 = cond1 && cmp_expr(expr1->ind2.x, expr2->ind2.x);
    }
    else
    {
      return cond1;
    }
    if (expr1->ind2.y)
    {
      cond3 = cond2 && cmp_expr(expr1->ind2.y, expr2->ind2.y);
    }
    else
    {
      return cond2;
    }
    if (expr1->ind2.z)
    {
      cond4 = cond3 && cmp_expr(expr1->ind2.z, expr2->ind2.z);
      return cond4;
    }
    else
    {
      return cond3;
    }
  }
  if (expr1->tag == VAR)
  {
    return (expr1->var.index == expr2->var.index);
  }
  if (expr1->tag == DEF)
  {
    return (!strcmp(expr1->def.name, expr2->def.name));
  }
  if (expr1->tag == TYPE)
  {
    return (expr1->type.level == expr2->type.level);
  }
  return false;
}

Expr* copy_expr(Expr* src)
{
  if (src == NULL)
    return NULL;
  Expr* dst = malloc(sizeof(Expr)); // no need for calloc here, zeros will be written if necessary
  dst->tag = src->tag;
  if (dst->tag < 2)
  {
    dst->var.name = strdup(src->var.name);
    dst->var.index = src->var.index;
    return dst;
  }
  if (dst->tag == TYPE)
  {
    dst->type.level = src->type.level;
    return dst;
  }
  dst->ind2.C = copy_expr(src->ind2.C);
  dst->ind2.x = copy_expr(src->ind2.x);
  dst->ind2.y = copy_expr(src->ind2.y);
  dst->ind2.z = copy_expr(src->ind2.z);
  return dst;
}

Expr* make_expr_0(tag_t tag)
{
  Expr* node = calloc(1, sizeof(Expr));
  node->tag = tag;
  return node;
}

Expr* make_expr_1(tag_t tag, Expr* arg1, bool copy)
{
  if (copy)
  {
    arg1 = copy_expr(arg1);
  }

  Expr* node = calloc(1, sizeof(Expr));
  node->tag = tag;
  node->fst.expr = arg1;
  return node;
}

Expr* make_expr_2(tag_t tag, Expr* arg1, Expr* arg2, bool copy)
{
  if (copy)
  {
    arg1 = copy_expr(arg1);
    arg2 = copy_expr(arg2);
  }

  Expr* node = calloc(1, sizeof(Expr));
  node->tag = tag;
  node->app.left = arg1;
  node->app.right = arg2;
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

  Expr* node = calloc(1, sizeof(Expr));
  node->tag = tag;
  node->rec.D = arg1;
  node->rec.tree_D = arg2;
  node->rec.t = arg3;
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

  Expr* node = calloc(1, sizeof(Expr));
  node->tag = tag;
  node->ind2.C = arg1;
  node->ind2.x = arg2;
  node->ind2.y = arg3;
  node->ind2.z = arg4;
  return node;
}

Expr* make_type(level_t level)
{
  Expr* node = calloc(1, sizeof(Expr));
  node->tag = TYPE;
  node->type.level = level;
  return node;
}
Expr* make_def(char* name, bool copy)
{
  Expr* node = calloc(1, sizeof(Expr));
  node->tag = DEF;
  if (copy)
    node->def.name = strdup(name);
  else
    node->def.name = name;
  return node;
}

Expr* make_var(debruijn id, char* name, bool copy)
{
  Expr* node = calloc(1, sizeof(Expr));
  node->tag = VAR;
  node->var.index = id;
  if (copy)
    node->var.name = strdup(name);
  else
    node->var.name = name;
  return node;
}

void free_expr(Expr* expr)
{
  if (!expr)
    return;
  if (expr->tag < 2)
  {
    free(expr->var.name);
    free(expr);
    return;
  }
  if (expr->tag == TYPE)
  {
    free(expr);
    return;
  }
  free_expr(expr->ind2.C);
  free_expr(expr->ind2.x);
  free_expr(expr->ind2.y);
  free_expr(expr->ind2.z);
  free(expr);
}

Expr* next_pi(Expr* expr)
{
  if (expr->tag == PI)
  {
    if (expr->pi.cod->tag == PI)
      return expr->pi.cod;
  }
  return NULL;
}

void free_var_stack(var_type_stack* vts)
{
  while (vts)
  {
    var_type_stack* temp = vts;
    vts = vts->next;
    if (temp->name)
      free(temp->name);
    if (temp->type)
      free_expr(temp->type);
    free(temp);
  }
}

void free_var_stack_until(var_type_stack* vts, var_type_stack* until)
{
  while (vts && vts != until)
  {
    var_type_stack* temp = vts;
    vts = vts->next;
    if (temp->name)
      free(temp->name);
    if (temp->type)
      free_expr(temp->type);
    free(temp);
  }
}

// it's only for parsing
var_type_stack* add_var_stack(var_type_stack* vts, char* s, Expr* ty)
{
  var_type_stack* new_vs = calloc(1, sizeof(var_type_stack));
  new_vs->next = vts;
  new_vs->name = (s); // TODO: make it optional to duplicate
  new_vs->type = (ty);
  return new_vs;
}

bool get_debruijn_1(debruijn step, var_type_stack* vts, char* s, debruijn* out)
{
  if (!vts)
    return false;
  if (!vts->name)
    return false;
  if (strcmp(vts->name, s) == 0)
  {
    *out = step;
    return true;
  }
  return get_debruijn_1(step + 1, vts->next, s, out);
}

bool get_debruijn(var_type_stack* vts, char* s, debruijn* out)
{
  return get_debruijn_1(0, vts, s, out);
}

void shift(Expr* expr, int d, int cutoff)
{
  if (!expr)
    return;
  if (expr->tag == VAR)
  {
    if (expr->var.index >= (debruijn)cutoff)
    {
      expr->var.index += d;
    }
  }
  else if (expr->tag <= TRUE_T)
  {
    return;
  }
  else if (expr->tag == APP)
  {
    shift(expr->app.left, d, cutoff);
    shift(expr->app.right, d, cutoff);
  }
  else if (expr->tag == LAM)
  {
    shift(expr->lam.type_annot, d, cutoff);
    shift(expr->lam.expr, d, cutoff + 1);
  }
  else if (expr->tag == PI)
  {
    shift(expr->pi.dom, d, cutoff);
    shift(expr->pi.cod, d, cutoff + 1);
  }
  else if (expr->tag == SIG)
  {
    shift(expr->sig.f_ty, d, cutoff);
    shift(expr->sig.family, d, cutoff + 1);
  }
  else if (expr->tag == PAIR)
  {
    shift(expr->pair.type_annot, d, cutoff);
    shift(expr->pair.a, d, cutoff);
    shift(expr->pair.b, d, cutoff);
  }
  else if (expr->tag == FST)
  {
    shift(expr->fst.expr, d, cutoff);
  }
  else if (expr->tag == SND)
  {
    shift(expr->snd.expr, d, cutoff);
  }
  else if (expr->tag == ID)
  {
    shift(expr->id.ty, d, cutoff);
    shift(expr->id.a, d, cutoff);
    shift(expr->id.b, d, cutoff);
  }
  else if (expr->tag == REFL)
  {
    shift(expr->refl.ty, d, cutoff);
    shift(expr->refl.a, d, cutoff);
  }
  else if (expr->tag == J)
  {
    shift(expr->j.t, d, cutoff);
    shift(expr->j.a, d, cutoff);
    shift(expr->j.b, d, cutoff);
    shift(expr->j.p, d, cutoff);
  }
  else if (expr->tag == W)
  {
    shift(expr->w.C, d, cutoff);
    shift(expr->w.family, d, cutoff + 1);
  }
  else if (expr->tag == TREE)
  {
    shift(expr->tree.root, d, cutoff);
    shift(expr->tree.subtr, d, cutoff);
  }
  else if (expr->tag == REC)
  {
    shift(expr->rec.D, d, cutoff);
    shift(expr->rec.tree_D, d, cutoff);
    shift(expr->rec.t, d, cutoff);
  }
  else if (expr->tag == IND0)
  {
    shift(expr->ind0.C, d, cutoff);
    shift(expr->ind0.x, d, cutoff);
  }
  else if (expr->tag == IND1)
  {
    shift(expr->ind1.C, d, cutoff);
    shift(expr->ind1.x, d, cutoff);
    shift(expr->ind1.y, d, cutoff);
  }
  else if (expr->tag == IND2)
  {
    shift(expr->ind2.C, d, cutoff);
    shift(expr->ind2.x, d, cutoff);
    shift(expr->ind2.y, d, cutoff);
    shift(expr->ind2.z, d, cutoff);
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
  if (expr->tag == VAR)
  {
    if (expr->var.index == n+acc)
    {
      return shift_expr(r, acc, 0);
    }
    else if (expr->var.index > n+acc)
    {
      // possible to optimize
      return copy_expr(expr); 
    }
    else
    {
      return copy_expr(expr);
    }
  }
  else if (expr->tag <= TRUE_T)
  {
    return copy_expr(expr);
  }
  else if (expr->tag == APP)
  {
    return make_app(subst_1(expr->app.left,n,r,acc), subst_1(expr->app.right,n,r, acc), false);
  }
  else if (expr->tag == LAM)
  {
    Expr* type_annot = subst_1(expr->lam.type_annot, n, r, acc);
    Expr* new_expr = subst_1(expr->lam.expr, n, r, acc+1);
    return make_lam(type_annot, new_expr, false);
  }
  else if (expr->tag == PI)
  {
    Expr* dom = subst_1(expr->pi.dom, n, r, acc);
    Expr* cod = subst_1(expr->pi.cod, n, r, acc+1);
    return make_pi(dom, cod, false);
  }
  else if (expr->tag == SIG)
  {
    Expr* f_ty = subst_1(expr->sig.f_ty, n, r, acc);
    Expr* family = subst_1(expr->sig.family, n, r, acc+1);
    return make_sig(f_ty, family, false);
  }
  else if (expr->tag == PAIR)
  {
    Expr* type_annot = subst_1(expr->pair.type_annot, n, r, acc);
    Expr* a = subst_1(expr->pair.a, n, r, acc);
    Expr* b = subst_1(expr->pair.b, n, r, acc);
    return make_pair(type_annot, a, b, false);
  }
  else if (expr->tag == FST)
  {
    return make_fst(subst_1(expr->fst.expr,n,r, acc), false);
  }
  else if (expr->tag == SND)
  {
    return make_snd(subst_1(expr->snd.expr,n,r, acc), false);
  }
  else if (expr->tag == ID)
  {
    Expr* ty = subst_1(expr->id.ty, n, r, acc);
    Expr* a = subst_1(expr->id.a, n, r, acc);
    Expr* b = subst_1(expr->id.b, n, r, acc);
    return make_id(ty, a, b, false);
  }
  else if (expr->tag == REFL)
  {
    Expr* ty = subst_1(expr->refl.ty, n, r, acc);
    Expr* a = subst_1(expr->refl.a, n, r, acc);
    return make_refl(ty, a, false);
  }
  else if (expr->tag == J)
  {
    Expr* t = subst_1(expr->j.t, n, r, acc);
    Expr* a = subst_1(expr->j.a, n, r, acc);
    Expr* b = subst_1(expr->j.b, n, r, acc);
    Expr* p = subst_1(expr->j.p, n, r, acc);
    return make_j(t, a, b, p, false);
  }
  else if (expr->tag == W)
  {
    Expr* C = subst_1(expr->w.C, n, r, acc);
    Expr* A = subst_1(expr->w.family, n, r, acc+1);
    return make_w(C, A, false);
  }
  else if (expr->tag == TREE)
  {
    Expr* root = subst_1(expr->tree.root, n, r, acc);
    Expr* subtr = subst_1(expr->tree.subtr, n, r, acc);
    return make_tree(root, subtr, false);
  }
  else if (expr->tag == REC)
  {
    Expr* D = subst_1(expr->rec.D, n, r, acc);
    Expr* tree_D = subst_1(expr->rec.tree_D, n, r, acc);
    Expr* t = subst_1(expr->rec.t, n, r, acc);
    return make_rec(D, tree_D, t, false);
  }
  else if (expr->tag == IND0)
  {
    Expr* C = subst_1(expr->ind0.C, n, r, acc);
    Expr* x = subst_1(expr->ind0.x, n, r, acc);
    return make_ind0(C, x, false);
  }
  else if (expr->tag == IND1)
  {
    Expr* C = subst_1(expr->ind1.C, n, r, acc);
    Expr* x = subst_1(expr->ind1.x, n, r, acc);
    Expr* y = subst_1(expr->ind1.y, n, r, acc);
    return make_ind1(C, x, y, false);
  }
  else if (expr->tag == IND2)
  {
    Expr* C = subst_1(expr->ind2.C, n, r, acc);
    Expr* x = subst_1(expr->ind2.x, n, r, acc);
    Expr* y = subst_1(expr->ind2.y, n, r, acc);
    Expr* z = subst_1(expr->ind2.z, n, r, acc);
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
