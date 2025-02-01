#pragma once

#include <stddef.h>
#include <stdint.h>
#include <ast.h>

#define MAX_NAME_SIZE 0x100
#define MAX_ARGS 0x100

void f_error(char* msg);
void parsing_error(char* msg);
void fail_msg(char* msg);
void type_error(char* msg);
void todo(char* msg);
