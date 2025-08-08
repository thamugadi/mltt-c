#include <print_ast.h>
#include <ast.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ast.h>

char* print_ast(Expr* expr)
{
  if (!expr)
    return NULL;

  if (GET_TAG(expr) == DEF)
  {
    return strdup(CAST_NAME(expr)->def.name);
  }
  if (GET_TAG(expr) == VAR)
  {
    char buffer[12];
    snprintf(buffer, sizeof(buffer), "$%u", CAST_NAME(expr)->var.index);
    return strdup(buffer);
  }
  if (GET_TAG(expr) == TYPE)
  {
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "(Type %u)", CAST_NAME(expr)->type.level);
    return strdup(buffer);
  }
  if (GET_TAG(expr) == ZERO)
  {
    return strdup("void");
  }
  if (GET_TAG(expr) == ONE)
  {
    return strdup("one");
  }
  if (GET_TAG(expr) == UNIT)
  {
    return strdup("unit");
  }
  if (GET_TAG(expr) == TWO)
  {
    return strdup("bool");
  }
  if (GET_TAG(expr) == FALSE_T)
  {
    return strdup("false");
  }
  if (GET_TAG(expr) == TRUE_T)
  {
    return strdup("true");
  }
  if (GET_TAG(expr) == APP)
  {
    char* left_str = print_ast(CAST_NAME(expr)->app.left);
    char* right_str = print_ast(CAST_NAME(expr)->app.right);
    size_t len = 3 + strlen(left_str) + strlen(right_str);
    char* result = malloc(len + 1);
    char* p = result;
    *p++ = '(';
    strcpy(p, left_str);
    p += strlen(left_str);
    *p++ = ' ';
    strcpy(p, right_str);
    p += strlen(right_str);
    *p++ = ')';
    *p = 0;
    free(left_str);
    free(right_str);
    return result;
  }
  if (GET_TAG(expr) == LAM)
  {
    char* expr_str = print_ast(CAST_NAME(expr)->lam.expr);
    char* result = NULL;
    result = malloc(7 + strlen(expr_str));
    sprintf(result, "(λ. %s)", expr_str);
    free(expr_str);
    return result;
  }
  if (GET_TAG(expr) == PI)
  {
    char* dom_str = print_ast(CAST_NAME(expr)->pi.dom);
    char* cod_str = print_ast(CAST_NAME(expr)->pi.cod);
    char* result = NULL;
    result = malloc(9 + strlen(dom_str) + strlen(cod_str));
    sprintf(result, "(Π %s, %s)", dom_str, cod_str);
    free(dom_str);
    free(cod_str);
    return result;
  }
  if (GET_TAG(expr) == SIG)
  {
    char* f_ty_str = print_ast(CAST_NAME(expr)->sig.f_ty);
    char* family_str = print_ast(CAST_NAME(expr)->sig.family);
    char* result = NULL;
    result = malloc(9 + strlen(f_ty_str) + strlen(family_str));
    sprintf(result, "(Σ %s, %s)", f_ty_str, family_str);
    free(f_ty_str);
    free(family_str);
    return result;
  }
  if (GET_TAG(expr) == PAIR)
  {
    char* x_str = print_ast(CAST_NAME(expr)->ind2.x);
    char* y_str = print_ast(CAST_NAME(expr)->ind2.y);
    char* C_str = print_ast(CAST_NAME(expr)->ind2.C);
    char* result = NULL;
    result = malloc(10 + strlen(x_str) + strlen(y_str) + strlen(C_str));
    sprintf(result, "([%s, %s] : %s)", x_str, y_str, C_str);
    free(x_str);
    free(y_str);
    free(C_str);
    return result;
  }
  if (GET_TAG(expr) == FST)
  {
    char* C_str = print_ast(CAST_NAME(expr)->ind2.C);
    char* result = NULL;
    result = malloc(8 + strlen(C_str));
    sprintf(result, "(fst %s)", C_str);
    free(C_str);
    return result;
  }
  if (GET_TAG(expr) == SND)
  {
    char* C_str = print_ast(CAST_NAME(expr)->ind2.C);
    char* result = NULL;
    result = malloc(8 + strlen(C_str));
    sprintf(result, "(snd %s)", C_str);
    free(C_str);
    return result;
  }
  if (GET_TAG(expr) == ID)
  {
    char* C_str = print_ast(CAST_NAME(expr)->ind2.C);
    char* x_str = print_ast(CAST_NAME(expr)->ind2.x);
    char* y_str = print_ast(CAST_NAME(expr)->ind2.y);
    char* result = NULL;
    result = malloc(9 + strlen(C_str) + strlen(x_str) + strlen(y_str));
    sprintf(result, "(eq %s %s %s)", C_str, x_str, y_str);
    free(C_str);
    free(x_str);
    free(y_str);
    return result;
  }
  if (GET_TAG(expr) == REFL)
  {
    char* C_str = print_ast(CAST_NAME(expr)->ind2.C);
    char* x_str = print_ast(CAST_NAME(expr)->ind2.x);
    char* result = NULL;
    result = malloc(9 + strlen(C_str) + strlen(x_str));
    sprintf(result, "(refl %s %s)", C_str, x_str);
    free(C_str);
    free(x_str);
    return result;
  }
  if (GET_TAG(expr) == J)
  {
    char* C_str = print_ast(CAST_NAME(expr)->j.C);
    char* c_str = print_ast(CAST_NAME(expr)->j.c);
    char* a_str = print_ast(CAST_NAME(expr)->j.a);
    char* b_str = print_ast(CAST_NAME(expr)->j.b);
    char* p_str = print_ast(CAST_NAME(expr)->j.p);
    char* result = NULL;
    result = malloc(9 + strlen(C_str) + strlen(c_str) + strlen(a_str) + strlen(b_str) + strlen(p_str));
    sprintf(result, "(J %s %s %s %s %s)", C_str, c_str, a_str, b_str, p_str);
    free(C_str);
    free(c_str);
    free(a_str);
    free(b_str);
    free(p_str);
    return result;
  }
  if (GET_TAG(expr) == W)
  {
    char* C_str = print_ast(CAST_NAME(expr)->ind2.C);
    char* x_str = print_ast(CAST_NAME(expr)->ind2.x);
    char* result = NULL;
    result = malloc(7 + strlen(C_str) + strlen(x_str));
    sprintf(result, "(W %s %s)", C_str, x_str);
    free(C_str);
    free(x_str);
    return result;
  }
  if (GET_TAG(expr) == TREE)
  {
    char* C_str = print_ast(CAST_NAME(expr)->ind2.C);
    char* x_str = print_ast(CAST_NAME(expr)->ind2.x);
    char* result = NULL;
    result = malloc(9 + strlen(C_str) + strlen(x_str));
    sprintf(result, "(tree %s %s)", C_str, x_str);
    free(C_str);
    free(x_str);
    return result;
  }
  if (GET_TAG(expr) == REC)
  {
    char* C_str = print_ast(CAST_NAME(expr)->ind2.C);
    char* x_str = print_ast(CAST_NAME(expr)->ind2.x);
    char* y_str = print_ast(CAST_NAME(expr)->ind2.y);
    char* result = NULL;
    result = malloc(10 + strlen(C_str) + strlen(x_str) + strlen(y_str));
    sprintf(result, "(rec %s %s %s)", C_str, x_str, y_str);
    free(C_str);
    free(x_str);
    free(y_str);
    return result;
  }
  if (GET_TAG(expr) == IND0)
  {
    char* C_str = print_ast(CAST_NAME(expr)->ind2.C);
    char* x_str = print_ast(CAST_NAME(expr)->ind2.x);
    char* result = NULL;
    result = malloc(9 + strlen(C_str) + strlen(x_str));
    sprintf(result, "(ind0 %s %s)", C_str, x_str);
    free(C_str);
    free(x_str);
    return result;
  }
  if (GET_TAG(expr) == IND1)
  {
    char* C_str = print_ast(CAST_NAME(expr)->ind2.C);
    char* x_str = print_ast(CAST_NAME(expr)->ind2.x);
    char* y_str = print_ast(CAST_NAME(expr)->ind2.y);
    char* result = NULL;
    result = malloc(11 + strlen(C_str) + strlen(x_str) + strlen(y_str));
    sprintf(result, "(ind1 %s %s %s)", C_str, x_str, y_str);
    free(C_str);
    free(x_str);
    free(y_str);
    return result;
  }
  if (GET_TAG(expr) == IND2)
  {
    char* C_str = print_ast(CAST_NAME(expr)->ind2.C);
    char* x_str = print_ast(CAST_NAME(expr)->ind2.x);
    char* y_str = print_ast(CAST_NAME(expr)->ind2.y);
    char* z_str = print_ast(CAST_NAME(expr)->ind2.z);
    char* result = NULL;
    result = malloc(12 + strlen(C_str) + strlen(x_str) + strlen(y_str) + strlen(z_str));
    sprintf(result, "(ind2 %s %s %s %s)", C_str, x_str, y_str, z_str);
    free(C_str);
    free(x_str);
    free(y_str);
    free(z_str);
    return result;
  }
  char buffer[64];
  snprintf(buffer, sizeof(buffer), "(unexpected tag: %d)", GET_TAG(expr));
  return strdup(buffer);
}
