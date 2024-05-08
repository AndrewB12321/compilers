
#ifndef PARAM_LIST_H
#define PARAM_LIST_H

#include "stmt.h"
#include "expr.h"
#include "decl.h"
#include "type.h"
#include "param_list.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
struct expr;

struct param_list {
	char *name;
	struct type *type;
	struct symbol *symbol;
	struct param_list *next;
};

struct param_list * param_list_create( char *name, struct type *type, struct param_list *next );
struct param_list * param_list_copy(struct param_list *p);
bool param_list_type_equals(struct param_list *a, struct param_list *b);
void param_list_print( struct param_list *a );
void param_list_resolve( struct param_list *a);
void param_list_delete( struct param_list *a );


#endif