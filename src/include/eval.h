#include <common.h>
#include <var_env.h>
#include <ast.h>

Expr* normalize(Expr* expr, var_env_t* var_env);
Expr* normalize_1(Expr* expr, var_env_t* var_env);
