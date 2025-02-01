#include <stdlib.h>
#include <string.h>
#include <ast.h>
#include <eval.h>
#include <parser_interface.h>
#include <def_env.h>
#include <var_env.h>
#include <type_check.h>
#include <common.h>

#include <stdio.h>
#include <print_ast.h>

Expr* normalize_1(Expr* expr, var_env_t* var_env);

Expr* normalize(Expr* expr, var_env_t* var_env)
{
  Expr* expr_type = type_infer(expr, var_env);
  if (!expr_type) return NULL;
  free_expr(expr_type);
  return normalize_1(expr, var_env);
}

// UB if not well-typed (i think?)
Expr* normalize_1(Expr* expr, var_env_t* var_env)
{
  if (expr->tag == DEF)
  {
    return copy_expr(get_term_hashmap(expr->def.name));
  }
  else if (expr->tag <= TRUE_T)
  {
    return copy_expr(expr);
  }
  else if (expr->tag == APP)
  {
    Expr* f = normalize_1(expr->app.left, var_env);
    Expr* arg = normalize_1(expr->app.right, var_env);
    Expr* replaced_expr = subst_top(f->lam.expr, arg);
    free_expr(f);
    free_expr(arg);
    Expr* normal_r_expr = normalize_1(replaced_expr, var_env);
    free_expr(replaced_expr);
    return normal_r_expr;
  }
  else if (expr->tag == LAM)
  {
    Expr* normal_annot = normalize_1(expr->lam.type_annot, var_env);
    Expr* normal_expr = normalize_1(expr->lam.expr, var_env);
    return make_lam(normal_annot, normal_expr, false);
  }
  else if (expr->tag == PI)
  {
    Expr* normal_dom = normalize_1(expr->pi.dom, var_env);
    Expr* normal_cod = normalize_1(expr->pi.cod, var_env);
    return make_pi(normal_dom, normal_cod, false);
  }
  else if (expr->tag == SIG)
  {
    Expr* normal_f_ty = normalize_1(expr->sig.f_ty, var_env);
    Expr* normal_family = normalize_1(expr->sig.family, var_env);
    return make_sig(normal_f_ty, normal_family, false);
  }
  else if (expr->tag == PAIR)
  {
    Expr* normal_left = normalize_1(expr->pair.a, var_env);
    Expr* normal_right = normalize_1(expr->pair.b, var_env);
    Expr* normal_annot = normalize_1(expr->pair.type_annot, var_env);
    return make_pair(normal_annot, normal_left, normal_right, false);
  }
  else if (expr->tag == FST)
  {
    Expr* normal_expr = normalize_1(expr->fst.expr, var_env);
    Expr* normal_left = normalize_1(normal_expr->pair.a, var_env);
    free_expr(normal_expr);
    return normal_left;
  }
  else if (expr->tag == SND)
  {
    Expr* normal_expr = normalize_1(expr->snd.expr, var_env);
    Expr* normal_right = normalize_1(normal_expr->pair.b, var_env);
    free_expr(normal_expr);
    return normal_right;
  }
  else if (expr->tag == ID)
  {
    Expr* normal_a = normalize_1(expr->id.a, var_env);
    Expr* normal_b = normalize_1(expr->id.b, var_env);
    Expr* normal_type = normalize_1(expr->id.ty, var_env);
    return make_id(normal_type, normal_a, normal_b, false);
  }
  else if (expr->tag == REFL)
  {
    Expr* normal_ty = normalize_1(expr->refl.ty, var_env);
    Expr* normal_expr = normalize_1(expr->refl.a, var_env);
    return make_refl(normal_ty, normal_expr, false);
  }
  else if (expr->tag == W)
  {
    Expr* normal_C = normalize_1(expr->w.C, var_env);
    Expr* normal_A = normalize_1(expr->w.family, var_env);
    return make_w(normal_C, normal_A, false);
  }
  else if (expr->tag == TREE)
  {
    Expr* root = expr->tree.root;
    Expr* subtr = expr->tree.subtr;
    Expr* normal_root = normalize_1(root, var_env);
    Expr* normal_subtr = normalize_1(subtr, var_env);
    return make_tree(normal_root, normal_subtr, false);
  }
  else if (expr->tag == J)
  {
    todo("todo:eval j");
  }
  else if (expr->tag == REC)
  {
    todo("todo:eval rec");
  }
  else if (expr->tag == IND0)
  {
    Expr* normal_C = normalize_1(expr->ind0.C, var_env);
    Expr* normal_x = normalize_1(expr->ind0.x, var_env);
    return make_ind0(normal_C, normal_x, false);
  }
  else if (expr->tag == IND1)
  {
    todo("todo:eval ind1");
  }
  else if (expr->tag == IND2)
  {
    Expr* normal_C = normalize_1(expr->ind2.C, var_env);
    Expr* normal_x = normalize_1(expr->ind2.x, var_env);
    Expr* normal_y = normalize_1(expr->ind2.y, var_env);
    Expr* normal_z = normalize_1(expr->ind2.z, var_env);
    
    if (normal_z->tag == FALSE_T)
    {
      free_expr(normal_z);
      free_expr(normal_y);
      free_expr(normal_C);
      return normal_x;
    }
    else if (normal_z->tag == TRUE_T)
    {
      free_expr(normal_z);
      free_expr(normal_x);
      free_expr(normal_C);
      return normal_y;
    }
    else
    {
      return make_ind2(normal_C, normal_x, normal_y, normal_z, false);
    }
  }
  f_error("shouldn't happen");
  exit(-1);
}
