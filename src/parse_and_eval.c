#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <eval.h>
#include <type_check.h>
#include <parser_interface.h>
#include <def_env.h>
#include <var_env.h>

#include <print_ast.h>
#include <parse_and_eval.h>

char* ARGS[MAX_ARGS];
void skip_spaces(char** s) { *s += strspn(*s, " \t\r\n\v\f"); }
bool read_identifier(char** s, char* dst, size_t n)
{
  if (!isalpha((unsigned char)**s) && **s != '_') return false;
  size_t len = 1 + strspn((*s) + 1, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_");
  if (len >= n) return false;
  memcpy(dst, *s, len);
  dst[len] = 0;
  *s += len;
  return true;
}
void free_args(uint32_t count)
{
  while (count--)
  {
    free(ARGS[count]);
    ARGS[count] = NULL;
  }
}

Expr* parse_line_expr(char** p, uint32_t arg_count, Expr* type)
{
  skip_spaces(p);
  char* line_end = *p + strcspn(*p, "\n");
  char saved = *line_end;
  *line_end = 0;
  Expr* expr = parse_expr(*p, arg_count, type);
  *line_end = saved;
  *p = *line_end ? line_end + 1 : line_end;
  return expr;
}

bool read_arguments(char** p, uint32_t* count)
{
  while (**p && **p != '=')
  {
    skip_spaces(p);
    if (!isalpha(**p) && **p != '_') break;
    char* start = *p;
    while (isalnum(**p) || **p == '_') (*p)++;
    size_t len = (size_t)(*p - start);
    if (!len) break;
    ARGS[*count] = calloc(1, len + 1);
    memcpy(ARGS[*count], start, len);
    ARGS[*count][len] = 0;
    (*count)++;
    if (*count >= MAX_ARGS)
    {
      free_args(*count);
      return false;
    }
    skip_spaces(p);
  }
  return true;
}

char* parse_and_eval_block(char** str)
{
  static char name1[256], name2[256]; // TODO: global
  char* p = *str;
  uint32_t arg_count = 0;
  Expr* type = NULL;
  Expr* normal_type = NULL;
  Expr* term = NULL;
  Expr* normal_term = NULL;

  skip_spaces(&p);
  if (!read_identifier(&p, name1, sizeof(name1))) goto error;
  skip_spaces(&p);
  if (*p != ':') goto error;
  p++;
  type = parse_line_expr(&p, 0, NULL);
  if (!type) goto error;
  skip_spaces(&p);
  if (!read_identifier(&p, name2, sizeof(name2))) goto error;
  if (strcmp(name1, name2)) goto error;
  skip_spaces(&p);
  if (!read_arguments(&p, &arg_count)) goto error;
  if (*p != '=') goto error;
  p++;
  normal_type = normalize(type, NULL);
  if (!normal_type) goto error;
  term = parse_line_expr(&p, arg_count, normal_type);
  if (!term) goto error;
  normal_term = normalize(term, NULL);
  if (!normal_term) goto error;
  if (!type_check(normal_term, normal_type, NULL))
  {
    // TODO: make it clear it's a typecheck fail
    goto error;
  }
  add_to_hashmap(name1, normal_term, normal_type);
  free_expr(type);
  free_expr(term);
  free_args(arg_count);
  *str = p;
  skip_spaces(str);

  printf("%s : ", name1);
  print_ast(get_type_hashmap(name1));
  printf("\n%s = ", name1);
  print_ast(get_term_hashmap(name1));
  printf("\n\n");

  return NULL;

error:
  while (*p)
  {
    if (*p != ':') p++;
    if (*p == ':')
    {
      p--;
      while (*p == ' ' || *p == '\n') p--;
      while (isalnum(*p) || *p == '_') p--;
      break;
    }
  }
  free_args(arg_count);
  free_expr(type);
  free_expr(normal_type);
  free_expr(term);
  free_expr(normal_term);
  *str = p;
  return name1;
}

char* remove_comments(char* code)
{
  char* src = code;
  char* dst = code;

  while (*src)
  {
    if (src[0] == '/' && src[1] == '/')
    {
      src += 2;
      while (*src && *src != '\n')
      {
        src++;
      }
    }
    else if (src[0] == '/' && src[1] == '*')
    {
      src += 2;
      while (*src && !(src[0] == '*' && src[1] == '/'))
      {
        src++;
      }
      if (*src)
      {
        src += 2;
      }
    }
    else
    {
      *dst++ = *src++;
    }
  }
  *dst = 0;
  return code;
}
