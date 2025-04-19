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

void run_repl()
{
  while(1);
}

int exec_arguments(int argc, char** argv)
{
  int opt;
  char* shortopts = "hf:o:r";
  long size;
  char* str = NULL;
  FILE* output_fp = NULL;
  bool repl = false;
  while ((opt = getopt(argc, argv, shortopts)) != -1)
  {
    if (opt == 'h')
    {
      printf("todo: help\n");
      exit(0);
    }
    if (opt == 'f')
    {
      str = get_file_str(optarg, &size);
    }
    if (opt == 'o')
    {
      if (access(optarg, F_OK) == 0)
      {
	printf("error: file %s already exists\n", optarg);
	exit(-1);
      }
      output_fp = fopen(optarg, "w");
    }
    if (opt == 'r')
    {
      repl = true;
    }
  }
  if (repl) // never stops
  {
    if (str) free(str);
    if (output_fp) fclose(output_fp);
    str = NULL;
    output_fp = NULL;
    run_repl();
  }

  if (!str)
  {
    if (optind < argc)
    {
      str = get_file_str(argv[optind], &size);
    }
    else
    {
      if (output_fp) fclose(output_fp);
      printf("error: no input file specified\n");
      exit(-1);
    }
  }

  init_hashmap();
  char* eval = eval_program(str, size);
  free_globals();
  free(str);

  if (output_fp)
  {
    fprintf(output_fp, "%s", eval);
    fclose(output_fp);
    free(eval);
    return 0;
  }
  else
  {
    printf("%s", eval);
    free(eval);
    return 0;
  }
  
}
int main(int argc, char** argv)
{
  if (argc < 2)
  {
    return 0;
  }
  int retval = exec_arguments(argc, argv);
  return retval;
}
