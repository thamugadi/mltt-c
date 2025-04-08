#pragma once
#include <stdbool.h>
char* parse_and_eval_block(char** str, char** result);
char* eval_program(char* str, int size);
char* remove_comments(char* code);
