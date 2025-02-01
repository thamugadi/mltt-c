#include <ast.h>
#include <var_env.h>
Expr* type_infer(Expr* term, var_env_t* var_env);
bool type_check(Expr *term, Expr *type, var_env_t* var_env);
