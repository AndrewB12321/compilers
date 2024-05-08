#ifndef EXPR_H
#define EXPR_H

#include "stmt.h"
#include "expr.h"
#include "decl.h"
#include "type.h"
#include "param_list.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum {
	EXPR_ADD,
	EXPR_SUB,
	EXPR_MULT,
	EXPR_DIV,
	EXPR_ASSIGN, // 4
	EXPR_OR,
	EXPR_AND,
	EXPR_LESSEQ,
	EXPR_LESS,
	EXPR_GREAT, // 9
	EXPR_EQ ,
	EXPR_NEQ,
	EXPR_GREATEQ,
	EXPR_MOD,
	EXPR_EXP, // 14
	EXPR_NEGATE,
	EXPR_UNARYSUB,
	EXPR_INC,
	EXPR_DEC,
	EXPR_NAME, // 19
	EXPR_BOOL_L,
	EXPR_CHAR_L, // 21
	EXPR_NUM_L,
	EXPR_STRING_L,
	EXPR_CALL, // 24
	EXPR_ARG,
	EXPR_SUBSCRIPT,
	EXPR_EXPR_LIST
	/* many more kinds of exprs to add here */
} expr_t;

struct expr {
	/* used by all kinds of exprs */
	expr_t kind;
	struct expr *left;
	struct expr *right;

	/* used by various leaf exprs */
	const char *name;
	int literal_value;
	const char * string_literal;
	struct symbol *symbol;

	int reg;
};

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right );

void expr_delete( struct expr *e );

struct expr * expr_create_name( const char *n );
struct expr * expr_create_integer_literal( int c );
struct expr * expr_create_boolean_literal( int c );
struct expr * expr_create_char_literal( char c );
struct expr * expr_create_string_literal( const char *str );

struct type * expr_typecheck( struct expr *e );


void expr_resolve( struct expr *e);

void expr_print( struct expr *e );

#endif