#ifndef CODEGEN_H
#define CODEGEN_H

#include "stmt.h"
#include "expr.h"
#include "decl.h"
#include "type.h"
#include "param_list.h"
#include "stack.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


struct scratch_register {
    char* name;
    bool inuse;
};

struct scratch_register** create_global_table();

int scratch_alloc();
void scratch_free( int r );
const char * scratch_name( int r );
const char * label_name( int label );

const char * symbol_codegen( struct symbol *s );
void decl_codegen(struct decl *d);
void expr_codegen(struct expr *e);
void stmt_codegen(struct stmt *s);

void delete_global_table();


#endif