
#ifndef DECL_H
#define DECL_H

#include "stmt.h"
#include "expr.h"
#include "decl.h"
#include "type.h"
#include "param_list.h"
#include "stack.h"
#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>


struct decl {
	char *name;
	struct type *type;
	struct expr *value;
	struct stmt *code;
	struct symbol *symbol;
	struct decl *next;
};

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next );
void decl_print( struct decl *d, int indent );
void decl_resolve( struct decl *d);
void decl_typecheck( struct decl *d );
void decl_delete( struct decl *e );

#endif