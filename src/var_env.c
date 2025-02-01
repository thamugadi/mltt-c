#include <ast.h>
#include <var_env.h>
#include <stdlib.h>
#include <stdio.h>
#include <print_ast.h>

void print_env(var_env_t* env)
{
  int i = 0;
  while (env)
  {
    printf("[%d]\n", i);
    if (env->expr) print_ast(env->expr);
    else printf("NULL");
    printf("\n");
    env = env->next;
    i++;
  }
}
int env_depth(var_env_t* env)
{
  int d = 0;
  while (env)
  {
    d++;
    env = env->next;
  }
  return d;
}
Expr* get_type(var_env_t* var_env, debruijn i)
{
  var_env_t* node = var_env;
  uint32_t idx = 0;
  while (node)
  {
    if (idx == (uint32_t)i)
    {
      Expr* copy = copy_expr(node->expr);
      return copy;
    }
    idx++;
    node = node->next;
  }
  return NULL;
}

inline var_env_t* add_type(Expr* expr, var_env_t* var_env)
{
  var_env_t* new_env = calloc(1, sizeof(var_env_t));
  new_env->expr = expr;
  new_env->next = var_env;
  return new_env;
}

inline void free_var_env_until(var_env_t* var_env, var_env_t* until)
{
  var_env_t* current = var_env;
  var_env_t* next_node;
  while (current != until)
  {
    next_node = current->next;
    free(current);
    current = next_node;
  }
}

inline void free_var_env(var_env_t* var_env)
{
  var_env_t* current = var_env;
  var_env_t* next_node;
  while (current != NULL)
  {
    next_node = current->next;
    free(current);
    current = next_node;
  }
}
