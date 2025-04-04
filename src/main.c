#include <ast.h>
#include <common.h>
#include <var_env.h>
#include <type_check.h>
#include <print_ast.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <parser_interface.h>
#include <parse_and_eval.h>
#include <stdio.h>
#include <ast.h>
#include <def_env.h>

extern def_env_t sym_hashmap[HASH_SIZE];

// this evaluates the whole program by dispatching blocks to parse_and_eval_block
char* eval_program(char* str, int size)
{
  // todo: rewrite this, optimized
  str[size] = 0;
  remove_comments(str);
  char** blocks = malloc(sizeof(char*));
  int i = 0;
  while (*str)
  {
    if (i) blocks = realloc(blocks, sizeof(char*) * (i + 1));
    char* error = parse_and_eval_block(&str, &blocks[i]);
    if (error)
    {
      char* errmsg = malloc(strlen(error) + 32);
      sprintf(errmsg, "** %s failed. **\n", error);
      for (int j = 0; j < i; j++) free(blocks[j]);
      free(blocks);
      return errmsg;
    }
    i++;
  }
  
  size_t len = 0;
  for (int j = 0; j < i; j++) len += strlen(blocks[j]);
  char* result = malloc(len + 1);
  result[0] = 0;
  for (int j = 0; j < i; j++)
  {
    strcat(result, blocks[j]);
    free(blocks[j]);
  }
  free(blocks); 
  return result;
}
void free_globals()
{
  for (int i = 0; i < HASH_SIZE; i++) free_def_entry(&sym_hashmap[i], false);
}

// todo: add multiple possible operations to do, not only loading a file
int main(int argc, char** argv)
{
  char* str;
  long size;
  if (argc != 2)
  {
    return 0;
  }
  FILE* fp = fopen(argv[1], "rb");
  if (!fp)
  {
    perror("no such file");
    return EXIT_FAILURE;
  }
  fseek(fp, 0, SEEK_END); 
  size = ftell(fp);
  if (size < 7)
  {
    fclose(fp);
    printf("something went wrong\n");
    return 0;
  }
  rewind(fp);
  str = malloc(size + 1);
  fread(str, 1, size, fp);

  init_hashmap();
  char* eval = eval_program(str, size);
  printf("%s", eval);

  free(eval);
  fclose(fp);
  free_globals();
  free(str);
  return 0;
}
