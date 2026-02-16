#pragma once
#include <common.h>
#include <stdbool.h>
#include <stdlib.h>
typedef uint16_t level_t;
typedef uint32_t debruijn;
typedef uint8_t tag_t;

typedef struct var_type_stack
{
  char* name;
  Expr* type;
  struct var_type_stack* next;
} var_type_stack;

void free_var_stack(var_type_stack* vts);
void free_var_stack_until(var_type_stack* vts, var_type_stack* until);
var_type_stack* add_var_stack(var_type_stack* vts, char* s, Expr* ty);

bool get_debruijn(var_type_stack* vts, char* s, debruijn* out);

Expr* next_pi(Expr* expr);
