#pragma once
#include <common.h>
#include <ast.h>
#include <stdint.h>

// must be <= 2^16
#define HASH_SIZE 0x10000

extern char* ARGS[MAX_ARGS];

bool check_valid_name(char c);

typedef struct def_env_t
{
  char* name;
  Expr* term;
  Expr* type;
  struct def_env_t* next;
} def_env_t;

void init_hashmap();
void free_globals();
void add_to_hashmap(char* name, Expr* term, Expr* type);
Expr* get_term_hashmap(char* name);
Expr* get_type_hashmap(char* name); //todo: merge them?

void free_def_entry(def_env_t* entry, bool was_allocated);
