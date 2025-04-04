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
int main(int argc, char** argv)
{
  char* str;
  long size;
  size_t read;
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
  if (fseek(fp, 0, SEEK_END) != 0)
  {
    fclose(fp);
    return EXIT_FAILURE;
  }
  size = ftell(fp);
  if (size < 0)
  {
    fclose(fp);
    return EXIT_FAILURE;
  }
  rewind(fp);
  str = malloc(size + 1);
  if (str == NULL)
  {
    fclose(fp);
    return EXIT_FAILURE;
  }
  read = fread(str, 1, size, fp);
  if (read != (size_t)size)
  {
    free(str);
    fclose(fp);
    return EXIT_FAILURE;
  }
  
  init_hashmap();
  char* eval = eval_program(str, size);
  printf("%s", eval);
  free(eval);
  fclose(fp);
  free_globals();
  free(str);
  return 0;
}
