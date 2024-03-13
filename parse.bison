
/*
Declare token types at the top of the bison file,
causing them to be automatically generated in parser.tab.h
for use by scanner.c.
*/

%token T_EOF
%token T_WHILE
%token T_PLUS
%token T_IDENT
%token T_NUMBER
%token T_ERROR
%token T_ARRAY
%token T_AUTO
%token T_BOOL
%token T_CHAR
%token T_CHARL //10
%token T_ELSE
%token T_FALSE
%token T_FUNCTION
%token T_TRUE
%token T_VOID
%token T_FOR
%token T_IF
%token T_INT
%token T_PRINT
%token T_STRING //20
%token T_STRINGL
%token T_RETURN
%token T_MINUS
%token T_MULT
%token T_DIV
%token T_ASSIGN
%token T_RPAREN
%token T_LPAREN
%token T_LBRACKET
%token T_RBRACKET //30
%token T_INCREMENT
%token T_DECREMENT
%token T_NEGATE
%token T_NOT
%token T_EXP
%token T_MOD
%token T_LESS
%token T_LESSEQ
%token T_GREAT
%token T_GREATEQ //40
%token T_EQ
%token T_NEQ
%token T_AND
%token T_OR
%token T_SCOLON
%token T_COLON
%token T_QUOTE
%token T_DQUOTE
%token T_RCBRACE
%token T_LCBRACE
%token T_COMMA


%{

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
Clunky: Manually declare the interface to the scanner generated by flex. 
*/

extern char *yytext;
extern int yylex();
extern int yyerror( char *str );

%}

%%

/* Here is the grammar: program is the start symbol. */

prog    : prog stmt  { return 0; }
        | stmt
	    ;

stmt    : expr T_SCOLON
        | decl
        ;

expr	: 
	    ;

decl    : def T_SCOLON
        | def T_ASSIGN T_IDENT T_SCOLON
        | def rtrn T_LPAREN param T_ASSIGN T_LCBRACE fncbody
        ;

rtrn    : type
        | T_VOID
        ;

param   : T_RPAREN
        | def T_RPAREN
        | def T_COMMA

fncbody : T_RCBRACE 
        | decl fncbody
        | expr fncbody
        ;


def     : T_IDENT T_SCOLON type

type    : T_CHAR
        | T_BOOL
        | T_AUTO
        | T_INT
        | T_STRING
        | T_FUNCTION
        ;

%%

/*
This function will be called by bison if the parse should
encounter an error.  In principle, "str" will contain something
useful.  In practice, it often does not.
*/

int yyerror( char *str )
{
	printf("parse error: %s\n",str);
	return 0;
}