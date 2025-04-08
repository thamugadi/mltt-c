#include <ast.h>
#include <common.h>
#include <var_env.h>
#include <type_check.h>
#include <print_ast.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <assert.h>

#include <parser_interface.h>
#include <parse_and_eval.h>
#include <stdio.h>
#include <ast.h>
#include <def_env.h>

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

void exec_arguments(int argc, char** argv)
{
  int opt;
  char* shortopts = "hf:o:r";
  //optarg
  while ((opt = getopt(argc, argv, shortopts)) != -1)
  {
    if (opt == 'h')
    {
      break;
    }
    if (opt == 'f')
    {
    }
    if (opt == 'o')
    {
    }
    if (opt == 'r')
    {
    }
  }
}

int main(int argc, char** argv)
{
  long size;
  if (argc < 2)
  {
    return 0;
  }

  char* str = get_file_str(argv[1], &size);
  init_hashmap();
  char* eval = eval_program(str, size);
  free_globals();
  free(str);

  printf("%s", eval);
  free(eval);

  return 0;
}
