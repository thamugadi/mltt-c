#include <common.h>
#include <stdio.h>
#include <stdlib.h>

void f_error(char* msg)
{
  printf("%s\n", msg);
  exit(-1);
}
void parsing_error(char* msg)
{
  printf("%s\n", msg);
  exit(-1);
}
void type_error(char* msg)
{
  printf("%s\n", msg);
  //exit(-2);
}
void inf_error(char* msg)
{
  printf("%s\n", msg);
  //exit(-3);
}
void todo(char* msg)
{
  printf("TODO: %s\n", msg);
  exit(-4);
}

void fail_msg(char* msg)
{
  printf("%s\n", msg);
}
