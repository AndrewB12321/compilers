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
#include "decl.h"
#include "expr.h"
#include "param_list.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "string.h"

// Flex
extern char *yytext;
extern int yylex();
extern int yyerror(char *str);

struct decl * parser_result = 0;

%}

/* Configure the semantic value type... */

%union {
	char* name;
	struct decl *decl;
	struct stmt *stmt;
	struct expr *expr;
	struct param_list *param_list;
	struct type *type;
	/* etc. */
	};

%type <decl> program global_statement_list global_statement decl_statement decl_expr init_statement 
			 init_expr func_decl_statement func_def_statement func_decl_expr
%type <stmt> statement_list statement block_statement if_statement loop_statement ret_statement print_statement
			 for_loop while_loop 
%type <expr> expr_list expr assign_expr or_expr and_expr rel_expr add_expr for_loop_param
			 mul_expr exp_expr unary_expr postfix_expr primary_expr array expr_list_tail expr_statement
			 subscript_expr subscript_expr_list subscript_expr_list_tail param_list param_list_tail 
%type <type> type array_spec formal_array_spec

%type <param_list>  formal_param_list formal_param formal_param_list_tail 

%type <name> indentifier

%%

program			
	: global_statement_list 
	{ parser_result = $1; }
	;

global_statement_list	
	: global_statement global_statement_list
	{ $$ = $1; $1->next = $2; }
	| %empty
	{ $$ = 0; }
	;

global_statement	
	: decl_statement
	{ $$ = $1; }
	| init_statement
	{ $$ = $1; }
	| func_decl_statement
	{ $$ = $1; }
	| func_def_statement
	{ $$ = $1; }
	;

statement_list	
	: statement statement_list
	{ $1->next = $2; 
	  $$ = $1; }
	| %empty
	{ $$ = 0; }
	;

statement			
	: block_statement
	{ $$ = $1; }
	| if_statement
	{ $$ = $1; }
	| expr_statement
	{ $$ = stmt_create(STMT_EXPR, 0, 0, $1, 0, 0, 0, 0); }
	| loop_statement
	{ $$ = $1; }
	| decl_statement
	{ $$ = stmt_create(STMT_DECL, $1, 0, 0, 0, 0, 0, 0); }
	| init_statement
	{ $$ = stmt_create(STMT_DECL, $1, 0, 0, 0, 0, 0, 0); }
	| func_decl_statement
	{ $$ = stmt_create(STMT_DECL, $1, 0, 0, 0, 0, 0, 0); }
	| func_def_statement
	{ $$ = stmt_create(STMT_DECL, $1, 0, 0, 0, 0, 0, 0); }
	| ret_statement
	{ $$ = $1; }
	| print_statement
	{ $$ = $1; }
	;

block_statement	
	: TOKEN_LCURLY statement_list TOKEN_RCURLY
	{ $$ = stmt_create(STMT_BLOCK, 0, 0, 0, 0, $2, 0, 0);  }
	;

if_statement	
	: TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN statement
	{ $$ = stmt_create(STMT_IF_ELSE, 0, 0, $3, 0, $5, 0, 0);  }
	| TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN statement TOKEN_ELSE statement
	{ $$ = stmt_create(STMT_IF_ELSE, 0, 0, $3, 0, $5, $7, 0);  }
	;

expr_statement	
	: expr TOKEN_SEMI
	{ $$ = $1; }
	;

expr			
	: assign_expr
	{ $$ = $1; }
	;

assign_expr		
	: assign_expr TOKEN_ASSIGN or_expr
	{ $$ = expr_create(EXPR_ASSIGN, $1, $3); }
	| or_expr
	{ $$ = $1; }
	;

or_expr			
	: or_expr TOKEN_OR and_expr
	{ $$ = expr_create(EXPR_OR, $1, $3); }
	| and_expr
	{ $$ = $1; }
	;

and_expr		
	: and_expr TOKEN_AND rel_expr
	{ $$ = expr_create(EXPR_AND, $1, $3); }
	| rel_expr
	{ $$ = $1; }
	;

rel_expr		
	: rel_expr TOKEN_LESS add_expr
	{ $$ = expr_create(EXPR_LESS, $1, $3); }
	| rel_expr TOKEN_LEQ add_expr
	{ $$ = expr_create(EXPR_LESSEQ, $1, $3); }
	| rel_expr TOKEN_GREATER add_expr
	{ $$ = expr_create(EXPR_GREAT, $1, $3); }
	| rel_expr TOKEN_GEQ add_expr
	{ $$ = expr_create(EXPR_GREATEQ, $1, $3); }
	| rel_expr TOKEN_EQ add_expr
	{ $$ = expr_create(EXPR_EQ, $1, $3); }
	| rel_expr TOKEN_NEQ add_expr
	{ $$ = expr_create(EXPR_NEQ, $1, $3); }
	| add_expr
	{ $$ = $1; }
	;

add_expr		
	: add_expr TOKEN_ADD mul_expr
	{ $$ = expr_create(EXPR_ADD, $1, $3); }
	| add_expr TOKEN_SUB mul_expr
	{ $$ = expr_create(EXPR_SUB, $1, $3); }
	| mul_expr
	{ $$ = $1; }
	;

mul_expr		
	: mul_expr TOKEN_MUL exp_expr 
	{ $$ = expr_create(EXPR_MULT, $1, $3); }
	| mul_expr TOKEN_DIV exp_expr 
	{ $$ = expr_create(EXPR_DIV, $1, $3); }
	| mul_expr TOKEN_MOD exp_expr 
	{ $$ = expr_create(EXPR_MOD, $1, $3); }
	| exp_expr 
	{ $$ = $1; }
	;

exp_expr		
	: exp_expr TOKEN_EXP unary_expr
	{ $$ = expr_create(EXPR_EXP, $1, $3); }
	| unary_expr
	{ $$ = $1; }
	;

unary_expr		
	: TOKEN_SUB unary_expr
	{ $$ = expr_create(EXPR_UNARYSUB, expr_create_integer_literal(0), $2); }
	| TOKEN_NOT unary_expr
	{ $$ = expr_create(EXPR_NEGATE, $2, 0); }
	| postfix_expr
	{ $$ = $1; }
	;

postfix_expr	
	: postfix_expr TOKEN_INC
	{ $$ = expr_create(EXPR_INC, $1, 0); }

	| postfix_expr TOKEN_DEC
	{ $$ = expr_create(EXPR_DEC, $1, 0); }
	| primary_expr
	{ $$ = $1; }
	;

primary_expr	
	: indentifier
	{ $$ = expr_create_name($1); }
	| TOKEN_NUMBER
	{ $$ = expr_create_integer_literal(atoi(yytext)); }
	| TOKEN_TRUE
	{ $$ = expr_create_boolean_literal(1); }
	| TOKEN_FALSE
	{ $$ = expr_create_boolean_literal(0); }
	| TOKEN_CHAR_LIT
	{ $$ = expr_create_char_literal(yytext[1]); }
	| TOKEN_STRING_LIT
	{ $$ = expr_create_string_literal(strdup(yytext)); } // FIX 
	| TOKEN_LPAREN expr TOKEN_RPAREN
	{ $$ = $2; }
	| indentifier subscript_expr_list
	{ $$ = expr_create(EXPR_SUBSCRIPT, expr_create_name($1), $2); }
	| indentifier TOKEN_LPAREN param_list TOKEN_RPAREN
	{ $$ = expr_create(EXPR_CALL, expr_create_name($1), $3); }
	| array
	{ $$ = $1; }
	;

indentifier
	: TOKEN_IDENT
	{ $$ = strdup(yytext); }
	;

subscript_expr_list 
	: subscript_expr subscript_expr_list_tail
	{ 
		if(!$2) $$ = $1;
		else $$ = expr_create(EXPR_SUBSCRIPT, $1, $2);
	}
	;

subscript_expr		
	: TOKEN_LBRACKET expr TOKEN_RBRACKET
	{ $$ = $2; }
	;

subscript_expr_list_tail	
	: subscript_expr subscript_expr_list_tail
	{ $$ = expr_create(EXPR_SUBSCRIPT, $1, $2); }
	| %empty
	{ $$ = 0; }
	;

type			
	: TOKEN_VOID
	{ $$ = type_create(TYPE_VOID, 0, 0); }
	| TOKEN_INTEGER
	{ $$ = type_create(TYPE_INTEGER, 0, 0); }
	| TOKEN_CHAR
	{ $$ = type_create(TYPE_CHARACTER, 0, 0); }
	| TOKEN_STRING
	{ $$ = type_create(TYPE_STRING, 0, 0); }
	| TOKEN_BOOLEAN
	{ $$ = type_create(TYPE_BOOLEAN, 0, 0); }
	| TOKEN_AUTO
	{ $$ = 0; }
	;

loop_statement	
	: for_loop
	{ $$ = $1; }
	| while_loop
	{ $$ = $1; }
	;

for_loop 		
	: TOKEN_FOR TOKEN_LPAREN for_loop_param TOKEN_SEMI for_loop_param TOKEN_SEMI for_loop_param TOKEN_RPAREN statement
	{ $$ = stmt_create(STMT_FOR, 0, $3, $5, $7, $9, 0, 0); }
	;

for_loop_param	
	: expr
	{ $$ = $1; }
	| %empty
	{ $$ = 0;}
	;

while_loop		
	: TOKEN_WHILE TOKEN_LPAREN expr TOKEN_RPAREN statement
	{ $$ = stmt_create(STMT_FOR, 0, 0, $3, 0, $5, 0, 0); }
	;

decl_statement	
	: decl_expr TOKEN_SEMI
	{ $$ = $1; }
	;

decl_expr		
	: indentifier TOKEN_COLON array_spec type
	{ 
		if($3) {
			struct type* tmp = $3;
			while(tmp->subtype) tmp = tmp->subtype;
			tmp->subtype = $4;
			$$ = decl_create($1, $3, 0, 0, 0);
		} else 
			$$ = decl_create($1, $4, 0, 0, 0);
	  
	}
	;

array_spec		
	: TOKEN_ARRAY TOKEN_LBRACKET TOKEN_NUMBER TOKEN_RBRACKET array_spec
	{ 
		if($5) $$ = type_create(TYPE_ARRAY, $5, 0); 
		else   $$ =	type_create(TYPE_ARRAY, 0, 0); 
	 
	}
	| TOKEN_ARRAY TOKEN_LBRACKET TOKEN_RBRACKET array_spec
	{ 
		if($4) $$ = type_create(TYPE_ARRAY, $4, 0); 
		else   $$ =	type_create(TYPE_ARRAY, 0, 0); 
	}
	| %empty
	{ $$ = 0; }
	;

init_statement	
	: init_expr TOKEN_SEMI
	{ $$ = $1; }
	;

init_expr		
	: decl_expr TOKEN_ASSIGN expr
	{ $1->value = $3; }
	;

func_decl_expr 	
	: indentifier TOKEN_COLON TOKEN_FUNCTION type TOKEN_LPAREN formal_param_list TOKEN_RPAREN
	{ $$ = decl_create($1, type_create(TYPE_FUNCTION, $4, $6), 0, 0, 0); }
	;

func_decl_statement	
	: func_decl_expr TOKEN_SEMI
	{ $$ = $1; }
	;

func_def_statement	
	: func_decl_expr TOKEN_ASSIGN block_statement
	{
		$1->code = $3;
		$$ = $1;
	}
	;

formal_param_list	
	: formal_param formal_param_list_tail
	{ $1->next = $2;
	  $$ = $1; }
	| %empty
	{ $$ = 0; }
	;

formal_param_list_tail	
	: TOKEN_COMMA formal_param formal_param_list_tail
	{ $2->next = $3; 
	  $$ = $2; }
	| %empty
	{ $$ = 0; }
	;

formal_param		
	: indentifier TOKEN_COLON formal_array_spec type
	{
		if($3) {
			struct type* tmp = $3;
			while(tmp->subtype) tmp = tmp->subtype;
			tmp->subtype = $4;
			$$ = param_list_create($1, $3, 0);
		} else 
			$$ = param_list_create($1, $4, 0);
	}
	;

formal_array_spec	
	: TOKEN_ARRAY TOKEN_LBRACKET TOKEN_RBRACKET formal_array_spec
	{
		if($4) $$ = type_create(TYPE_ARRAY, $4, 0); 
		else   $$ =	type_create(TYPE_ARRAY, 0, 0); 
	}
	| %empty
	{ $$ = 0; }
	;

ret_statement		
	: TOKEN_RETURN expr TOKEN_SEMI
	{ $$ = stmt_create(STMT_RETURN, 0, 0, $2, 0, 0, 0, 0); }
	| TOKEN_RETURN TOKEN_SEMI
	{ $$ = stmt_create(STMT_RETURN, 0, 0, 0, 0, 0, 0, 0); }
	;

param_list			
	: expr param_list_tail
	{ $$ = expr_create(EXPR_ARG, $1, $2); }
	| %empty
	{ $$ = 0; }
	;

param_list_tail		
	: TOKEN_COMMA expr param_list_tail
	{ $$ = expr_create(EXPR_ARG, $2, $3); }
	| %empty
	{ $$ = 0; }
	;

print_statement		
	: TOKEN_PRINT expr_list TOKEN_SEMI
	{ $$ = stmt_create(STMT_PRINT, 0, 0, $2, 0, 0, 0, 0); }
	| TOKEN_PRINT TOKEN_SEMI
	{ $$ = stmt_create(STMT_RETURN, 0, 0, 0, 0, 0, 0, 0); }
	;

expr_list			
	: expr expr_list_tail
	{
	 $$ = expr_create(EXPR_EXPR_LIST, $1, $2);
	}
	;

expr_list_tail		
	: TOKEN_COMMA expr_list
	{ $$ = $2; }
	| %empty
	{ $$ = 0;}
	;

array				
	: TOKEN_LCURLY expr_list TOKEN_RCURLY
	{ $$ = $2; }
	;

%%

int yyerror(char *str) {
	printf("[PARSE ERROR]: %s\n",str);
	return 0;
}
