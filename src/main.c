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
#include <eval.h>
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

void help()
{
  printf("todo: write help\n");
}

void run_repl()
{
  // todo: improve it, add definitions etc
  char repl_buffer[0x1000];
  init_hashmap();
  while(1)
  {
    fgets(repl_buffer, 0x1000, stdin);
    Expr* expr = parse_expr(repl_buffer, 0, NULL);
    if (!expr)
    {
      printf("failed.\n");
      continue;
    }
    Expr* ty = type_infer(expr, NULL);
    if (!ty)
    {
      printf("failed.\n");
      free_expr(expr);
      continue;
    }
    else
    {
      Expr* eval_expr = normalize_1(expr, NULL);
      char* eval_expr_str = print_ast(eval_expr);
      printf("%s", eval_expr_str);
      free(eval_expr_str);
      printf(" : ");
      char* ty_str = print_ast(ty);
      printf("%s\n", ty_str);
      free(ty_str);
      free_expr(ty);
      free_expr(eval_expr);
      free_expr(expr);
    }
  }
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
      help();
      return 0;
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
	return -1;
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
      return -1;
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
    help();
    return 0;
  }
  int retval = exec_arguments(argc, argv);
  return retval;
}
