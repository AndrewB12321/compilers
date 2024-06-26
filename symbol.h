
#ifndef SYMBOL_H
#define SYMBOL_H

#include "stmt.h"
#include "expr.h"
#include "decl.h"
#include "type.h"
#include "param_list.h"
#include "symbol.h"
#include "hash_table.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {
	SYMBOL_LOCAL,
	SYMBOL_PARAM,
	SYMBOL_GLOBAL
} symbol_t;

struct symbol {
	symbol_t kind;
	struct type *type;
	char *name;
	int which;
};

struct symbol * symbol_create( symbol_t kind, struct type *type, char *name );
struct symbol * symbol_copy( struct symbol *s );
void symbol_delete(struct symbol* s);
void scope_enter();
void scope_exit();
int scope_level();
void scope_bind(const char* name, struct symbol *sym);
struct symbol *scope_lookup(const char *name);
struct symbol *scope_lookup_current(const char *name);

#endif