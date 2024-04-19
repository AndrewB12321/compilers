#include "stmt.h"
#include "expr.h"
#include "decl.h"
#include "type.h"
#include "param_list.h"
#include "indent.h"
#include <stdio.h>
#include <stdlib.h>

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params ) {
    struct type *tmp = malloc(sizeof(*tmp));
    tmp->kind = kind;
    tmp->subtype = subtype;
    tmp->params = params;
    return tmp;
}

void type_print( struct type *t ) {
    if(!t) return;
    switch(t->kind){
        case TYPE_BOOLEAN:
            printf("bool");
            break;
        case TYPE_INTEGER:
            printf("integer");
            break;
        case TYPE_STRING:
            printf("string");
            break;
        case TYPE_CHARACTER:
            printf("char");
            break;
        case TYPE_VOID:
            printf("void");
            break;
        case TYPE_ARRAY:
            printf("array[]");
            type_print(t->subtype);
            break;
        case TYPE_FUNCTION:
            printf("function");
            type_print(t->subtype);
            printf("(");
            param_list_print(t->params);
            printf(")");

            break;

    }
}

void type_delete( struct type *t ) {
    if(!t) return;
    param_list_delete(t->params);
    type_delete(t->subtype);
    free(t);
}