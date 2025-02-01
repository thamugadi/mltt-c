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

int main(int argc, char** argv)
{
  char* str;
  long size;
  size_t read;
  if (argc != 2)
    return 0;
  init_hashmap();
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
  char* initial_str = str;
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
  str[size] = 0;
  fclose(fp);
  remove_comments(str);

//  char* tmp_str = str;
  while (*str)
  {
    char* ident = parse_and_eval_block(&str);
    if (!ident) continue;
    printf("** %s failed. **\n", ident);
    exit(0);
/*  if (str == tmp_str)
    {
      printf("unexpected error (todo: make it more clear)\n");
      break;
    }
    else
    {
      tmp_str = str;
    }*/
    // todo: indicate it if it failed because of a metasyntax error
  }

/*for (int i = 0; i < HASH_SIZE; i++)
  {
    def_env_t* entry = &sym_hashmap[i];
    while (entry->name)
    {
      char* name = entry->name;
      printf("%s : ", name);
      print_ast(entry->type);
      printf("\n%s = ", name);
      print_ast(entry->term);
      printf("\n\n");
      if (entry->next) entry = entry->next;
      else break; 
    }
  }*/
  
  for (int i = 0; i < HASH_SIZE; i++) free_def_entry(&sym_hashmap[i], false);
  free(initial_str);
  return 0;
}
