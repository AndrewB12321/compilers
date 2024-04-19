#include "stmt.h"
#include "expr.h"
#include "decl.h"
#include "type.h"
#include "param_list.h"
#include "indent.h"
#include <stdio.h>
#include <stdlib.h>

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
    print_indent(indent);
    printf("%s", d->name);
    printf(":");
    type_print(d->type);
    if(d->value) {
        printf(" = ");
        expr_print(d->value);
    };
    if(d->type->kind != TYPE_FUNCTION) printf(";");
    else {
        printf("{\n");
        stmt_print(d->code, indent + 1, 0);
        printf("}");
    }
    printf("\n");
    //printf("\n\n ***************NEXT DECL************\n\n");
    decl_print(d->next, indent);

    

}

void decl_delete( struct decl *e ) {
    if(!e) return;
    type_delete(e->type);
    expr_delete(e->value);
    stmt_delete(e->code);
    //if(e->symbol) symbol_delete(e->symbol);  // symbol not implemented yet
    decl_delete(e->next);
    free(e);
}


