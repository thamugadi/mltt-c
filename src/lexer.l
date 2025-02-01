%{
#include <parser.tab.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
%}
%option never-interactive
%option nounput
%option noinput
%%
[ \t\n\r]+
"("                     { return T_LPAREN; }
")"                     { return T_RPAREN; }
":"                     { return T_COLON; }
","                     { return T_COMMA; }
"lambda"                { return T_LAMBDA; }
"forall"                { return T_FORALL; }
"exists"                { return T_EXISTS; }
"["                     { return T_LBRACK; }
"]"                     { return T_RBRACK; }
"Type"                  { return T_TYPE; }
"fst"                   { return T_FST; }
"snd"                   { return T_SND; }
"eq"                    { return T_EQ; }
"refl"                  { return T_REFL; }
"J"                     { return T_J; }
"W"                     { return T_W; }
"tree"                  { return T_TREE; }
"rec"                   { return T_REC; }
"ind0"                  { return T_IND0; }
"ind1"                  { return T_IND1; }
"ind2"                  { return T_IND2; }
"void"                  { return T_BOTTOM; }
"one"                   { return T_ONE; }
"bool"                  { return T_BOOL; }
"unit"                  { return T_UNIT; }
"false"                 { return T_FALSE; }
"true"                  { return T_TRUE; }
"."                     { return T_DOT; }
[0-9]+                  { yylval.ival = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]*       { yylval.sval = strdup(yytext); return IDENT; }
[0-9]+[a-zA-Z_][a-zA-Z0-9_]*  { yylval.sval = strdup(yytext); return IDENT; }
<<EOF>>                 { return 0; }
.                       { return yytext[0]; }
%%
int yywrap() { return 1; }
