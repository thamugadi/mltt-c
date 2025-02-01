#pragma once
#include <common.h>
#include <ast.h>

typedef struct var_env_t
{
  Expr* expr;
  struct var_env_t* next;
} var_env_t;

void print_env(var_env_t* env);
Expr* get_type(var_env_t* var_env, debruijn i);
var_env_t* add_type(Expr* expr, var_env_t* var_env);
void free_var_env_until(var_env_t* var_env, var_env_t* until);
void free_var_env(var_env_t* var_env);


int env_depth(var_env_t* env);
