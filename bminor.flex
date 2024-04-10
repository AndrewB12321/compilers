%{
#include "parser.h"
%}

DIGIT  [0-9]
LETTER [a-zA-Z]

%option yylineno

%%
(" "|\t|\n)                                     /* Skip whitespace */
"//".*                                          /* Inline comments */
"/*"([^*]|(\*+[^*/]))*\*+\/                     /* Block comments */
\+                                              { return TOKEN_ADD; }
-                                               { return TOKEN_SUB; }
!                                               { return TOKEN_NOT; }
\+\+                                            { return TOKEN_INC; }
--                                              { return TOKEN_DEC; }
\^                                              { return TOKEN_EXP; }
\*                                              { return TOKEN_MUL; }
\/                                              { return TOKEN_DIV; }
\%                                              { return TOKEN_MOD; }
\<                                              { return TOKEN_LESS; }
\<=                                             { return TOKEN_LEQ; }
\>                                              { return TOKEN_GREATER; }
\>=                                             { return TOKEN_GEQ; }
==                                              { return TOKEN_EQ; }
!=                                              { return TOKEN_NEQ; }
&&                                              { return TOKEN_AND; }
\|\|                                            { return TOKEN_OR; }
=                                               { return TOKEN_ASSIGN; }
array                                           { return TOKEN_ARRAY; }
auto                                            { return TOKEN_AUTO; }
boolean                                         { return TOKEN_BOOLEAN; }
char                                            { return TOKEN_CHAR; }
else                                            { return TOKEN_ELSE; }
false                                           { return TOKEN_FALSE; }
for                                             { return TOKEN_FOR; }
function                                        { return TOKEN_FUNCTION; }
if                                              { return TOKEN_IF; }
integer                                         { return TOKEN_INTEGER; }
print                                           { return TOKEN_PRINT; }
return                                          { return TOKEN_RETURN; }
string                                          { return TOKEN_STRING; }
true                                            { return TOKEN_TRUE; }
void                                            { return TOKEN_VOID; }
while                                           { return TOKEN_WHILE; }
'([^'\\\n]|\\.)'                                { return TOKEN_CHAR_LIT; }
\"(\\.|[^"\\])*\"                               { return TOKEN_STRING_LIT; }
({LETTER}|_)({LETTER}|{DIGIT}|_)*               { return TOKEN_IDENT; }
{DIGIT}+                                        { return TOKEN_NUMBER; }
:                                               { return TOKEN_COLON; }
;                                               { return TOKEN_SEMI; }
,                                               { return TOKEN_COMMA; }
\(                                              { return TOKEN_LPAREN; }
\)                                              { return TOKEN_RPAREN; }
\[                                              { return TOKEN_LBRACKET; }
\]                                              { return TOKEN_RBRACKET; }
\{                                              { return TOKEN_LCURLY; }
\}                                              { return TOKEN_RCURLY; }
.                                               { return TOKEN_ERROR; }
%%

int yywrap() { return 1; }
