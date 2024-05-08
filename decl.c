#include "stmt.h"
#include "expr.h"
#include "decl.h"
#include "type.h"
#include "param_list.h"
#include "stack.h"
#include "symbol.h"
#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>

extern struct stack* global_stack;
extern struct scratch_register* global_register_Table;
extern FILE* global_output_file;

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next ) {
    struct decl *d = malloc(sizeof(*d));
    d->name = name;
    d->type = type;
    d->value = value;
    d->code = code;
    d->next = next;
    return d;
}

void decl_print( struct decl *d, int indent) {
    if(!d) return;
    printf("%s", d->name);
    printf(":");
    type_print(d->type);
    if(d->value) {
        printf(" = ");
        expr_print(d->value);
    };
    if(!d->code) printf(";");
    else {
        stmt_print(d->code, indent + 1);
    }
    printf("\n");
    //printf("\n\n ***************NEXT DECL************\n\n");
    decl_print(d->next, indent);

    

}

void decl_resolve(struct decl *d) {
    if(!d) return;
    symbol_t kind = scope_level() > 1 ? SYMBOL_LOCAL : SYMBOL_GLOBAL;
    d->symbol = symbol_create(kind, d->type, d->name);
    expr_resolve(d->value);
    scope_bind(d->name, d->symbol);
    if(d->code) {
        scope_enter();
        param_list_resolve(d->type->params);
        stmt_resolve(d->code);
        scope_exit();
    }
    decl_resolve(d->next);
}

void decl_typecheck( struct decl *d ) {
    if(!d) return;
    if(d->value) {
        struct type *t;
        t = expr_typecheck(d->value);
        if(!type_equals(t,d->symbol->type)) {
        /* display an error */
        }
    }
    if(d->code) {
        stmt_typecheck(d->code);
    }
    decl_typecheck(d->next);    
}


void decl_delete( struct decl *e ) {
    if(!e) return;
    type_delete(e->type);
    expr_delete(e->value);
    stmt_delete(e->code);
    if(e->symbol) symbol_delete(e->symbol); 
    decl_delete(e->next);
    free(e);
}


