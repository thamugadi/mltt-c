#pragma once
#include <stdbool.h>
char* parse_and_eval_block(char** str, char** result);

char* remove_comments(char* code);
