#include <def_env.h>
#include <ast.h>
#include <string.h>
#include <stdio.h>
#include <print_ast.h>
uint16_t (*hash_function)(char*);
def_env_t sym_hashmap[HASH_SIZE];

uint16_t crc_16(char* name)
{
  int length = strlen(name);
  uint8_t i;
  uint32_t data;
  uint32_t crc = 0xffff;
  if (!length) return (~crc);
  do
  {
    for (i = 0, data = (uint32_t)0xff & *name++; i < 8; i++, data >>= 1)
    {
      if ((crc & 1) ^ (data & 1)) crc = (crc >> 1) ^ 0x8408;
      else crc >>= 1;
    }
  } while (--length);
  crc = ~crc;
  data = crc;
  crc = (crc << 8) | (data >> 8 & 0xff);
  #if HASH_SIZE==0x10000
  return crc;
  #else
  return crc % HASH_SIZE;
  #endif
}

void init_hashmap()
{
  hash_function = crc_16;
  memset(sym_hashmap, 0, HASH_SIZE*sizeof(def_env_t)); 
}

void free_globals()
{
  for (int i = 0; i < HASH_SIZE; i++)
  {
    free_def_entry(&sym_hashmap[i], false);
  }
}
void add_to_hashmap(char* name, Expr* term, Expr* type)
{
  uint16_t hash = hash_function(name);
  def_env_t* entry = &sym_hashmap[hash];
  while (entry->name)
  {
    entry->next = calloc(1, sizeof(def_env_t));
    entry = entry->next;
  }
  entry->name = strdup(name);
  entry->term = term;
  entry->type = type;
}

// no copy

Expr* get_term_hashmap(char* name)
{
  uint16_t hash = hash_function(name);
  def_env_t* entry = &sym_hashmap[hash];
  if (!entry->name) return NULL;
  while (1)
  {
    if (!entry->name) return NULL;
    if (!strcmp(entry->name, name)) return entry->term;
    entry = entry->next;
  }
}

// no copy

Expr* get_type_hashmap(char* name)
{
  uint16_t hash = hash_function(name);
  def_env_t* entry = &sym_hashmap[hash];
  while (1)
  {
    if (!entry->name) return NULL;
    if (!strcmp(entry->name, name)) return entry->type;
    entry = entry->next;
  }
}
void free_def_entry(def_env_t* entry, bool was_allocated)
{
  if (!entry) return;
  if (entry->name)
  {
    free(entry->name);
    free_expr(entry->type);
    free_expr(entry->term);
  }
  free_def_entry(entry->next, true);
  if (was_allocated) free(entry);
}
