/* Tokens */
%token TOKEN_EOF
%token TOKEN_ARRAY
%token TOKEN_AUTO
%token TOKEN_BOOLEAN
%token TOKEN_CHAR
%token TOKEN_FALSE
%token TOKEN_FOR
%token TOKEN_FUNCTION
%token TOKEN_IF
%token TOKEN_INTEGER
%token TOKEN_PRINT
%token TOKEN_RETURN
%token TOKEN_STRING
%token TOKEN_TRUE
%token TOKEN_VOID
%token TOKEN_WHILE
%token TOKEN_CHAR_LIT
%token TOKEN_STRING_LIT
%token TOKEN_ADD
%token TOKEN_SUB
%token TOKEN_NOT
%token TOKEN_INC
%token TOKEN_DEC
%token TOKEN_IDENT
%token TOKEN_EXP
%token TOKEN_MUL
%token TOKEN_DIV
%token TOKEN_MOD
%token TOKEN_LESS
%token TOKEN_LEQ
%token TOKEN_GREATER
%token TOKEN_GEQ
%token TOKEN_EQ
%token TOKEN_NEQ
%token TOKEN_AND
%token TOKEN_OR
%token TOKEN_ASSIGN
%token TOKEN_NUMBER
%token TOKEN_COLON
%token TOKEN_SEMI
%token TOKEN_COMMA
%token TOKEN_LPAREN
%token TOKEN_LBRACKET
%token TOKEN_RBRACKET
%token TOKEN_LCURLY
%token TOKEN_RCURLY
%token TOKEN_ERROR

/* Establish precedence for ')' and "else" to resolve dangling else problem */
%nonassoc TOKEN_RPAREN
%nonassoc TOKEN_ELSE

%{

// Includes
#include <stdio.h>
#include <stdlib.h>

// Flex
extern int yylex();
extern int yyerror(char *str);

%}

%%

program			
	: global_statement_list 
	;

global_statement_list	
	: global_statement global_statement_list
	| %empty
	;

global_statement	
	: decl_statement
	| init_statement
	| func_decl_statement
	| func_def_statement
	;

statement_list	
	: statement statement_list
	| %empty
	;

statement			
	: block_statement
	| if_statement
	| expr_statement
	| loop_statement
	| decl_statement
	| init_statement
	| func_decl_statement
	| func_def_statement
	| ret_statement
	| print_statement
	;

block_statement	
	: TOKEN_LCURLY statement_list TOKEN_RCURLY
	;

if_statement	
	: TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN statement
	| TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN statement TOKEN_ELSE statement
	;

expr_statement	
	: expr TOKEN_SEMI
	;

expr			
	: assign_expr
	;

assign_expr		
	: assign_expr TOKEN_ASSIGN or_expr
	| or_expr
	;

or_expr			
	: or_expr TOKEN_OR and_expr
	| and_expr
	;

and_expr		
	: and_expr TOKEN_AND rel_expr
	| rel_expr
	;

rel_expr		
	: rel_expr TOKEN_LESS add_expr
	| rel_expr TOKEN_LEQ add_expr
	| rel_expr TOKEN_GREATER add_expr
	| rel_expr TOKEN_GEQ add_expr
	| rel_expr TOKEN_EQ add_expr
	| rel_expr TOKEN_NEQ add_expr
	| add_expr
	;

add_expr		
	: add_expr TOKEN_ADD mul_expr
	| add_expr TOKEN_SUB mul_expr
	| mul_expr
	;

mul_expr		
	: mul_expr TOKEN_MUL exp_expr 
	| mul_expr TOKEN_DIV exp_expr 
	| mul_expr TOKEN_MOD exp_expr 
	| exp_expr 
	;

exp_expr		
	: exp_expr TOKEN_EXP unary_expr
	| unary_expr
	;

unary_expr		
	: TOKEN_SUB unary_expr
	| TOKEN_NOT unary_expr
	| postfix_expr
	;

postfix_expr	
	: postfix_expr TOKEN_INC
	| postfix_expr TOKEN_DEC
	| primary_expr
	;

primary_expr	
	: TOKEN_IDENT
	| TOKEN_NUMBER
	| TOKEN_TRUE
	| TOKEN_FALSE
	| TOKEN_CHAR_LIT
	| TOKEN_STRING_LIT
	| TOKEN_LPAREN expr TOKEN_RPAREN
	| TOKEN_IDENT subscript_expr_list
	| TOKEN_IDENT TOKEN_LPAREN param_list TOKEN_RPAREN
	| array
	;

subscript_expr_list 
	: subscript_expr subscript_expr_list_tail
	;

subscript_expr		
	: TOKEN_LBRACKET expr TOKEN_RBRACKET
	;

subscript_expr_list_tail	
	: subscript_expr subscript_expr_list_tail
	| %empty
	;

type			
	: TOKEN_VOID
	| TOKEN_INTEGER
	| TOKEN_CHAR
	| TOKEN_STRING
	| TOKEN_BOOLEAN
	| TOKEN_AUTO
	;

loop_statement	
	: for_loop
	| while_loop
	;

for_loop 		
	: TOKEN_FOR TOKEN_LPAREN for_loop_param TOKEN_SEMI for_loop_param TOKEN_SEMI for_loop_param TOKEN_RPAREN statement
	;

for_loop_param	
	: expr
	| %empty
	;

while_loop		
	: TOKEN_WHILE TOKEN_LPAREN expr TOKEN_RPAREN statement
	;

decl_statement	
	: decl_expr TOKEN_SEMI
	;

decl_expr		
	: TOKEN_IDENT TOKEN_COLON array_spec type
	;

array_spec		
	: TOKEN_ARRAY TOKEN_LBRACKET TOKEN_NUMBER TOKEN_RBRACKET array_spec
	| %empty
	;

init_statement	
	: init_expr TOKEN_SEMI
	;

init_expr		
	: decl_expr TOKEN_ASSIGN expr
	;

func_decl_expr 	
	: TOKEN_IDENT TOKEN_COLON TOKEN_FUNCTION type TOKEN_LPAREN formal_param_list TOKEN_RPAREN
	;

func_decl_statement	
	: func_decl_expr TOKEN_SEMI
	;

func_def_statement	
	: func_decl_expr TOKEN_ASSIGN block_statement
	;

formal_param_list	
	: formal_param formal_param_list_tail
	| %empty
	;

formal_param_list_tail	
	: TOKEN_COMMA formal_param formal_param_list_tail
	| %empty
	;

formal_param		
	: TOKEN_IDENT TOKEN_COLON formal_array_spec type
	;

formal_array_spec	
	: TOKEN_ARRAY TOKEN_LBRACKET TOKEN_RBRACKET formal_array_spec
	| %empty
	;

ret_statement		
	: TOKEN_RETURN expr TOKEN_SEMI
	| TOKEN_RETURN TOKEN_SEMI
	;

param_list			
	: expr param_list_tail
	| %empty
	;

param_list_tail		
	: TOKEN_COMMA expr param_list_tail
	| %empty
	;

print_statement		
	: TOKEN_PRINT expr_list TOKEN_SEMI
	| TOKEN_PRINT TOKEN_SEMI
	;

expr_list			
	: expr expr_list_tail
	;

expr_list_tail		
	: TOKEN_COMMA expr_list
	| %empty
	;

array				
	: TOKEN_LCURLY expr_list TOKEN_RCURLY
	;

%%

int yyerror(char *str) {
	printf("[PARSE ERROR]: %s\n",str);
	return 0;
}