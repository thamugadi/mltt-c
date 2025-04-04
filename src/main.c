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
  int n_blocks = 0;
  char* str_1 = str;
  while (*str_1) if (*str_1++ == '=') n_blocks++; // can do better to compute the number of blocks, ig 
  
  char** blocks = malloc(n_blocks*sizeof(char*));
  // while (*str)
  for (int i = 0; i < n_blocks; i++)
  {
    // str gets updated to next block
    // blocks[i] contains the resulting block
    char* error = parse_and_eval_block(&str, &blocks[i]);
    if (error)
    {
      char* errmsg = malloc(strlen(error) + 17);
      sprintf(errmsg, "** %s failed. **\n", error);
      for (int j = 0; j < i; j++) free(blocks[j]);
      free(blocks);
      return errmsg;
    }
  }
  size_t len = 0;
  for (int i = 0; i < n_blocks; i++) len += strlen(blocks[i]);
  char* result = malloc(len + 1);
  result[0] = 0;
  for (int j = 0; j < n_blocks; j++)
  {
    strcat(result, blocks[j]);
    free(blocks[j]);
  }
  free(blocks); 
  return result;
}

void free_globals()
{
  for (int i = 0; i < HASH_SIZE; i++)
  {
    free_def_entry(&sym_hashmap[i], false);
  }
}

char* get_file_str(char* filename, long* size_ptr)
{
  FILE* fp = fopen(filename, "rb");
  char* str;
  long size;
  if (!fp)
  {
    perror("no such file");
    return NULL;
  }
  fseek(fp, 0, SEEK_END); 
  size = ftell(fp);
  if (size < 7)
  {
    fclose(fp);
    printf("something went wrong\n");
    return NULL;
  }
  rewind(fp);
  str = malloc(size + 1);
  fread(str, 1, size, fp);
  fclose(fp);
  *size_ptr = size;
  return str;
}

// todo: add multiple possible operations to do, not only loading a file
int main(int argc, char** argv)
{
  long size;
  if (argc != 2)
  {
    return 0;
  }

  char* str = get_file_str(argv[1], &size);
  if (!str)
  {
    return 0;
  }

  init_hashmap();
  char* eval = eval_program(str, size);
  printf("%s", eval);
  free(eval);
  free_globals();
  free(str);

  return 0;
}
