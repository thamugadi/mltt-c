#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <ast.h>

Expr* parse_expr(char* str, uint32_t arg_count, Expr* ty);
