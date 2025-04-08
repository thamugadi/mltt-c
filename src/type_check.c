#include <ast.h>
#include <common.h>
#include <def_env.h>
#include <eval.h>
#include <type_check.h>
#include <var_env.h>
#include <print_ast.h>

#include <stdlib.h>
#include <stdio.h>

//todo: free new env in error case

#define FAIL_IF_NOT(cond, msg)                                                 \
  do                                                                           \
  {                                                                            \
    if (!(cond))							\
    {                                                                          \
      fail_msg(msg);                                                           \
      goto error;                                                              \
    }                                                                          \
  } while (0)

Expr* type_infer_zero() { return make_type(0); }
Expr* type_infer_one() { return make_type(0); }
Expr* type_infer_two() { return make_type(0); }
Expr* type_infer_unit() { return make_one(); }
Expr* type_infer_false() { return make_two(); }
Expr* type_infer_true() { return make_two(); }

Expr* type_infer_type(Expr* type) { return make_type(1 + type->type.level); }

Expr* type_infer_var(Expr* var, var_env_t* var_env)
{
  Expr* t;
  t = get_type(var_env, var->var.index);
  shift(t, var->var.index + 1, 0);
  FAIL_IF_NOT(t, "todo:msg_1");
  return t;
error:
  return NULL;
}
Expr* type_infer_def(Expr* var, var_env_t* var_env)
{
  Expr* type = get_type_hashmap(var->def.name);
  FAIL_IF_NOT(type, "todo:msg_2");
  return copy_expr(type);
error:
  return NULL;
}

Expr* type_infer_lam(Expr* lam, var_env_t* var_env)
{
  Expr* type_annot = lam->lam.type_annot;
  FAIL_IF_NOT(type_check(lam, type_annot, var_env),
              "incorrect annotation (todo:msg_3)");
  Expr* normal_annot = normalize_1(type_annot, var_env);
  FAIL_IF_NOT(normal_annot, "todo:msg_4");
  return normal_annot;
error:
  
  return NULL;
}

Expr* type_infer_pair(Expr* pair, var_env_t* var_env)
{
  Expr* type_annot = pair->pair.type_annot;
  FAIL_IF_NOT(type_check(pair, type_annot, var_env),
              "incorrect annotation (todo:msg_5)");
  Expr* normal_annot = normalize_1(type_annot, var_env);
  FAIL_IF_NOT(normal_annot, "todo:msg_6");
  return normal_annot;
error:
  return NULL;
}

Expr* type_infer_app(Expr* app, var_env_t* var_env)
{
  Expr* f_type = NULL;
  Expr* res_type = NULL;
  Expr* normal_res_type = NULL;
  f_type = type_infer(app->app.left, var_env);
  FAIL_IF_NOT(f_type, "todo:1001");
  FAIL_IF_NOT(f_type->tag == PI, "todo:1002");
  
  bool res = type_check(app->app.right, f_type->pi.dom, var_env);
  FAIL_IF_NOT(res, "todo:1004");

  res_type = subst_top(f_type->pi.cod, app->app.right);
  free_expr(f_type);
  normal_res_type = normalize_1(res_type, var_env);
  
  free_expr(res_type);
  return normal_res_type;
error:
  if (f_type) free_expr(f_type);
  if (res_type) free_expr(res_type);
  if (normal_res_type) free_expr(normal_res_type);
  return NULL;
}

Expr* type_infer_pi(Expr* pi, var_env_t* var_env)
{
  Expr* dom_type = NULL;
  Expr* normal_dom = NULL;
  var_env_t* new_env = NULL;
  Expr* cod_type = NULL;

  dom_type = type_infer(pi->pi.dom, var_env);
  FAIL_IF_NOT(dom_type, "todo:msg_11");
  FAIL_IF_NOT(dom_type->tag == TYPE, "todo:msg_12");

  normal_dom = normalize_1(pi->pi.dom, var_env);
  FAIL_IF_NOT(normal_dom, "todo:msg_13");

  new_env = add_type(normal_dom, var_env);
  FAIL_IF_NOT(new_env, "todo:msg_14");

  cod_type = type_infer(pi->pi.cod, new_env);
  FAIL_IF_NOT(cod_type, "todo:msg_15");
  FAIL_IF_NOT(cod_type->tag == TYPE, "todo:msg_16");

  level_t lv = (dom_type->type.level > cod_type->type.level)
                   ? dom_type->type.level
                   : cod_type->type.level;
  free_var_env_until(new_env, var_env);
  free_expr(dom_type);
  free_expr(normal_dom);
  free_expr(cod_type);
  return make_type(lv);

error:
  if (new_env)
    free_var_env_until(new_env, var_env);
  if (dom_type)
    free_expr(dom_type);
  if (normal_dom)
    free_expr(normal_dom);
  if (cod_type)
    free_expr(cod_type);
  return NULL;
}

Expr* type_infer_sig(Expr* sig, var_env_t* var_env)
{
  Expr* fty_type = NULL;
  Expr* normal_fty = NULL;
  var_env_t* new_env = NULL;
  Expr* family_type = NULL;

  fty_type = type_infer(sig->sig.f_ty, var_env);
  FAIL_IF_NOT(fty_type, "todo:msg_17");
  FAIL_IF_NOT(fty_type->tag == TYPE, "todo:msg_18");

  normal_fty = normalize_1(sig->sig.f_ty, var_env);
  FAIL_IF_NOT(normal_fty, "todo:msg_19");

  new_env = add_type(normal_fty, var_env);
  FAIL_IF_NOT(new_env, "todo:msg_20");

  family_type = type_infer(sig->sig.family, new_env);
  FAIL_IF_NOT(family_type, "todo:msg_21");
  FAIL_IF_NOT(family_type->tag == TYPE, "todo:msg_22");

  level_t lv = (fty_type->type.level > family_type->type.level)
                   ? fty_type->type.level
                   : family_type->type.level;
  free_var_env_until(new_env, var_env);
  free_expr(fty_type);
  free_expr(normal_fty);
  free_expr(family_type);
  return make_type(lv);

error:
  if (new_env)
    free_var_env_until(new_env, var_env);
  if (fty_type)
    free_expr(fty_type);
  if (normal_fty)
    free_expr(normal_fty);
  if (family_type)
    free_expr(family_type);
  return NULL;
}

Expr* type_infer_fst(Expr* expr, var_env_t* var_env)
{
  Expr* ty_e = NULL;
  Expr* f_ty = NULL;

  ty_e = type_infer(expr->fst.expr, var_env);
  FAIL_IF_NOT(ty_e, "todo:msg_23");
  FAIL_IF_NOT(ty_e->tag == SIG, "fst");

  f_ty = copy_expr(ty_e->sig.f_ty);
  FAIL_IF_NOT(f_ty, "todo:msg_24");
  free_expr(ty_e);
  return f_ty;

error:
  if (ty_e)
    free_expr(ty_e);
  if (f_ty)
    free_expr(f_ty);
  return NULL;
}

Expr* type_infer_snd(Expr* expr, var_env_t* var_env)
{
  Expr* ty_e = NULL;
  Expr* fst_e = NULL;
  Expr* normal_fst = NULL;
  Expr* fst_e_type = NULL;
  Expr* family_applied = NULL;
  Expr* normal_family_applied = NULL;

  ty_e = type_infer(expr->snd.expr, var_env);
  FAIL_IF_NOT(ty_e, "todo:msg_25");
  FAIL_IF_NOT(ty_e->tag == SIG, "snd");

  fst_e = make_fst(expr->snd.expr, true);
  FAIL_IF_NOT(fst_e, "todo:msg_26");

  normal_fst = normalize_1(fst_e, var_env);
  FAIL_IF_NOT(normal_fst, "todo:msg_27");
  free_expr(fst_e);

  fst_e_type = type_infer(normal_fst, var_env);
  FAIL_IF_NOT(fst_e_type, "todo:msg_28");
  free_expr(fst_e_type);

  family_applied = subst_top(ty_e->sig.family, normal_fst);
  free_expr(normal_fst);
  free_expr(ty_e);
  normal_family_applied = normalize_1(family_applied, var_env);

  free_expr(family_applied);
  return normal_family_applied;

error:
  if (family_applied) free_expr(family_applied);
  if (fst_e_type) free_expr(fst_e_type);
  if (normal_fst) free_expr(normal_fst);
  if (fst_e) free_expr(fst_e);
  if (ty_e) free_expr(ty_e);
  return NULL;
}

Expr* type_infer_refl(Expr* refl, var_env_t* var_env)
{
  Expr* ty = NULL;
  Expr* a = NULL;
  Expr* b = NULL;
  Expr* r = NULL;

  ty = copy_expr(refl->refl.ty);
  FAIL_IF_NOT(ty, "todo:msg_31");

  a = copy_expr(refl->refl.a);
  FAIL_IF_NOT(a, "todo:msg_32");

  b = copy_expr(a);
  FAIL_IF_NOT(b, "todo:msg_33");

  FAIL_IF_NOT(type_check(a, ty, var_env), "todo:msg_34");

  r = make_id(ty, a, b, true);
  FAIL_IF_NOT(r, "todo:msg_35");

  free_expr(ty);
  free_expr(a);
  free_expr(b);
  return r;

error:
  if (r)
    free_expr(r);
  if (ty)
    free_expr(ty);
  if (a)
    free_expr(a);
  if (b)
    free_expr(b);
  return NULL;
}

Expr* type_infer_w(Expr* w, var_env_t* var_env)
{
  Expr* type_C = NULL;
  Expr* normal_C = NULL;
  Expr* A_type = NULL;
  var_env_t* new_env = NULL;

  type_C = type_infer(w->w.C, var_env);
  FAIL_IF_NOT(type_C, "todo:msg_36");
  FAIL_IF_NOT(type_C->tag == TYPE, "todo:msg_37");

  normal_C = normalize_1(w->w.C, var_env);
  new_env = add_type(normal_C, var_env);

  A_type = type_infer(w->w.family, new_env);
  FAIL_IF_NOT(A_type->tag == TYPE, "todo:msg_5000");
  level_t lv = (type_C->type.level > A_type->type.level)
                   ? type_C->type.level
                   : A_type->type.level;
  free_var_env_until(new_env, var_env);
  free_expr(type_C);
  free_expr(normal_C);
  free_expr(A_type);

  return make_type(lv);
  
error:
  if (new_env)
    free_var_env_until(new_env, var_env);
  if (type_C)
    free_expr(type_C);
  if (normal_C)
    free_expr(normal_C);
  if (A_type)
    free_expr(A_type);
  return NULL;
}

Expr* type_infer_tree(Expr* tree, var_env_t* var_env)
{
  // todo: rewrite all of this. not clean enough and bad denomination, also probably incomplete checking
  
  Expr* type_C = NULL;
  Expr* subtr_type = NULL;
  Expr* normal_C = NULL;
  Expr* type_A = NULL;
  Expr* res_type = NULL;
  Expr* normal_res_type = NULL;
  Expr* Aroot = NULL;
  Expr* normal_Aroot = NULL;
  var_env_t* new_env = NULL;

  type_C = type_infer(tree->tree.root, var_env);
  FAIL_IF_NOT(type_C, "todo:msg_45");

  subtr_type = type_infer(tree->tree.subtr, var_env);
  FAIL_IF_NOT(subtr_type, "todo:msg_46");

  normal_C = normalize_1(type_C, var_env);
  FAIL_IF_NOT(normal_C, "todo:msg_47");
  FAIL_IF_NOT(subtr_type->tag == PI, "todo:msg_48");
  FAIL_IF_NOT(subtr_type->pi.cod->tag == W, "todo:msg_49");
  free_expr(type_C);

  new_env = add_type(subtr_type->pi.cod->w.C, var_env);
  type_A = type_infer(subtr_type->pi.cod->w.family, new_env);
  free_var_env_until(new_env, var_env);

  FAIL_IF_NOT(type_A, "todo:msg_51");
  FAIL_IF_NOT(cmp_expr(normal_C, subtr_type->pi.cod->w.C), 
              "todo:msg_52");
  free_expr(type_A);
  free_expr(normal_C);
  
  Aroot = make_app(subtr_type->pi.cod->w.family, tree->tree.root, true);
  normal_Aroot = normalize_1(Aroot, var_env);
  free_expr(Aroot);
  FAIL_IF_NOT(cmp_expr(normal_Aroot, subtr_type->pi.dom), "todo:msg_521");
  free_expr(normal_Aroot);

  res_type = copy_expr(subtr_type->pi.cod);
  FAIL_IF_NOT(res_type, "todo:msg_53");
  free_expr(subtr_type);
  
  normal_res_type = normalize_1(res_type, var_env);
  FAIL_IF_NOT(normal_res_type, "todo:msg_54");

  free_expr(res_type);
  return normal_res_type;
error:
  if (Aroot) free_expr(Aroot);
  if (normal_Aroot) free_expr(normal_Aroot);
  if (new_env) free_var_env_until(new_env, var_env);
  if (type_C) free_expr(type_C);
  if (normal_C) free_expr(normal_C);
  if (subtr_type) free_expr(subtr_type);
  if (type_A) free_expr(type_A);
  if (res_type) free_expr(res_type);
  if (normal_res_type) free_expr(normal_res_type);
  return NULL;
}
Expr* type_infer_id(Expr* id, var_env_t* var_env)
{
  Expr* type = id->id.ty;
  FAIL_IF_NOT(type_check(id->id.a, type, var_env), "todo:msg_58");
  FAIL_IF_NOT(type_check(id->id.b, type, var_env), "todo:msg_59");
  return type_infer(type, var_env);
error:
  return NULL;
}

Expr* type_infer_j(Expr* j, var_env_t* var_env)
{
  todo("type_infer_j");
  return NULL;
}
Expr* type_infer_rec(Expr* rec, var_env_t* var_env)
{
  /*
    tree_D : Π (c : C) 
           (subt : A(c) → W_{c:C} A(c)) 
           (subt_D : Π (a : A(c)) D(subt(a))), 
             D(tree(c, subt))
  */

  todo("rec");
  
  return NULL;
error:
  return NULL;
}

Expr* type_infer_ind0(Expr* ind0, var_env_t* var_env)
{
  Expr* x_type = NULL;
  Expr* C_type = type_infer(ind0->ind0.C, var_env);
  FAIL_IF_NOT(C_type, "todo:msg_60");
  FAIL_IF_NOT(C_type->tag == PI, "todo:msg_61");

  FAIL_IF_NOT((C_type->pi.dom)->tag == ZERO, "todo:msg_62");
  FAIL_IF_NOT((C_type->pi.cod)->tag == TYPE, "todo:msg_63");
  free_expr(C_type);
  
  x_type = type_infer(ind0->ind0.x, var_env);
  FAIL_IF_NOT(x_type->tag == ZERO, "todo:msg_64");
  free_expr(x_type);

  Expr* app = make_app(ind0->ind0.C, ind0->ind0.x, true);
  Expr* normal_app = normalize_1(app, var_env);

  free_expr(app);
  return normal_app;

error:
  if (C_type) free_expr(C_type);
  if (x_type) free_expr(x_type);
  return NULL;
}

Expr* type_infer_ind1(Expr* ind1, var_env_t* var_env)
{
  todo("type_infer_ind1");
  return NULL;
}

Expr* type_infer_ind2(Expr* ind2, var_env_t* var_env)
{
  Expr* C_type = NULL;
  Expr* x_type = NULL;
  Expr* y_type = NULL;
  Expr* z_type = NULL;

  Expr* C_false = NULL;
  Expr* normal_C_false = NULL;
  Expr* C_true = NULL;
  Expr* normal_C_true = NULL;
  Expr* normal_dom = NULL;
  Expr* C_z = NULL; 

  C_type = type_infer(ind2->ind2.C, var_env);
  FAIL_IF_NOT(C_type, "todo:msg_65");
  FAIL_IF_NOT(C_type->tag == PI, "todo:msg_66");
  
  FAIL_IF_NOT((C_type->pi.dom)->tag == TWO, "todo:msg_67");
  FAIL_IF_NOT((C_type->pi.cod)->tag == TYPE, "todo:msg_68");

  x_type = type_infer(ind2->ind2.x, var_env);

  C_false = make_app(copy_expr(ind2->ind2.C), make_false(), false);
  
  normal_C_false = normalize_1(C_false, var_env);
  FAIL_IF_NOT(cmp_expr(x_type, normal_C_false), "todo:msg_69");
  free_expr(normal_C_false);
  free_expr(C_false);
  free_expr(x_type);
  
  y_type = type_infer(ind2->ind2.y, var_env);

  C_true = make_app(copy_expr(ind2->ind2.C), make_true(), false);
  
  normal_C_true = normalize_1(C_true, var_env);
  FAIL_IF_NOT(cmp_expr(y_type, normal_C_true), "todo:msg_70");
  free_expr(normal_C_true);
  free_expr(C_true);
  free_expr(y_type);

  z_type = type_infer(ind2->ind2.z, var_env);
  FAIL_IF_NOT(z_type->tag == TWO, "todo:msg_71");

  normal_dom = normalize_1(C_type->pi.dom, var_env);
  FAIL_IF_NOT(cmp_expr(z_type, normal_dom), "todo:msg_ind2err_z");
  free_expr(normal_dom);
  free_expr(C_type);
  free_expr(z_type);

  C_z = make_app(ind2->ind2.C, ind2->ind2.z, true);
  Expr* normal_C_z = normalize_1(C_z, var_env);

  free_expr(C_z);
  return normal_C_z;
error:
  if (C_type) free_expr(C_type);
  if (z_type) free_expr(z_type);
  if (C_false) free_expr(C_false);
  if (C_true) free_expr(C_true);
  if (normal_C_false) free_expr(normal_C_false);
  if (normal_C_true) free_expr(normal_C_true);
  if (normal_dom) free_expr(normal_C_true);
  return NULL;
}

Expr* type_infer(Expr* term, var_env_t* var_env)
{
  if (term->tag == ZERO)
  {
    return type_infer_zero();
  }
  else if (term->tag == ONE)
  {
    return type_infer_one();
  }
  else if (term->tag == TWO)
  {
    return type_infer_two();
  }
  else if (term->tag == UNIT)
  {
    return type_infer_unit();
  }
  else if (term->tag == FALSE_T)
  {
    return type_infer_false();
  }
  else if (term->tag == TRUE_T)
  {
    return type_infer_true();
  }
  else if (term->tag == TYPE)
  {
    return type_infer_type(term);
  }
  else if (term->tag == VAR)
  {
    return type_infer_var(term, var_env);
  }
  else if (term->tag == DEF)
  {
    return type_infer_def(term, var_env);
  }
  else if (term->tag == ID)
  {
    return type_infer_id(term, var_env);
  }
  else if (term->tag == REFL)
  {
    return type_infer_refl(term, var_env);
  }
  else if (term->tag == APP)
  {
    return type_infer_app(term, var_env);
  }
  else if (term->tag == PAIR)
  {
    return type_infer_pair(term, var_env);
  }
  else if (term->tag == LAM)
  {
    return type_infer_lam(term, var_env);
  }
  else if (term->tag == PI)
  {
    return type_infer_pi(term, var_env);
  }
  else if (term->tag == SIG)
  {
    return type_infer_sig(term, var_env);
  }
  else if (term->tag == FST)
  {
    return type_infer_fst(term, var_env);
  }
  else if (term->tag == SND)
  {
    return type_infer_snd(term, var_env);
  }
  else if (term->tag == J)
  {
    return type_infer_j(term, var_env);
  }
  else if (term->tag == W)
  {
    return type_infer_w(term, var_env);
  }
  else if (term->tag == TREE)
  {
    return type_infer_tree(term, var_env);
  }
  else if (term->tag == REC)
  {
    return type_infer_rec(term, var_env);
  }
  else if (term->tag == IND0)
  {
    return type_infer_ind0(term, var_env);
  }
  else if (term->tag == IND1)
  {
    return type_infer_ind1(term, var_env);
  }
  else if (term->tag == IND2)
  {
    return type_infer_ind2(term, var_env);
  }
  else
  {
    fail_msg("shouldn't be reached");
    return NULL;
  }
}

bool type_check(Expr* term, Expr* type, var_env_t* var_env)
{
  if (term->tag == LAM)
  {
    if (type->tag != PI)
    {
      return false;
    }
    Expr* dom = type->pi.dom;
    Expr* cod = type->pi.cod;
    Expr* normal_dom = normalize(dom, var_env);
    var_env_t* new_env = add_type(normal_dom, var_env);

    Expr* normal_cod = normalize(cod, new_env);

    bool res = type_check(term->lam.expr, normal_cod, new_env);

    free_var_env_until(new_env, var_env);
    free_expr(normal_dom);
    free_expr(normal_cod);
    return res;
  }
  else if (term->tag == PAIR)
  {
    if (type->tag != SIG)
    {
      return false;
    }
    Expr* f_ty = type->sig.f_ty;
    Expr* family = type->sig.family;
    Expr* normal_f_ty = normalize(f_ty, var_env);
    Expr* normal_family_app;
    if (!type_check(term->pair.a, normal_f_ty, var_env))
    {
      free_expr(normal_f_ty);
      return false;
    }
    Expr* family_app = subst_top(family, term->pair.a);
    normal_family_app = normalize_1(family_app, var_env);
    
    bool res = type_check(term->pair.b, normal_family_app, var_env);
    free_expr(normal_family_app);
    free_expr(family_app);
    free_expr(normal_f_ty);
    return res;
  }
  else if (term->tag == TYPE)
  {
    if (type->tag == TYPE && type->type.level == term->type.level + 1)
    {
      return true;
    }
    return false;
  }
  else
  {
    Expr* type_inf = type_infer(term, var_env);
    if (!type_inf) return false;
    bool eq = cmp_expr(type, type_inf);
    free_expr(type_inf);
    return eq;
  } 
}
