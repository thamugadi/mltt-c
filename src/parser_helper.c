#include <parser_helper.h>
#include <ast.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
this contains operations on the AST that i defined when writing parser.y

it's mostly a matter of making the named -> debruijn conversion

better to read parser.y to understand what it is for

*/



// skips a pi-type
Expr* next_pi(Expr* expr)
{
  if (GET_TAG(expr) == PI)
  {
    if (GET_TAG(CAST_NAME(expr)->pi.cod) == PI)
    {
      return CAST_NAME(expr)->pi.cod;
    }
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
  new_vs->name = s; // TODO: make it optional to duplicate? idk if it'd ever be useful
  new_vs->type = ty;
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

