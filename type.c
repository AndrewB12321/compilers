#include "stmt.h"
#include "expr.h"
#include "decl.h"
#include "type.h"
#include "param_list.h"
#include "symbol.h"
#include "codegen.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern struct stack* global_stack;

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
            printf("function ");
            type_print(t->subtype);
            printf(" (");
            param_list_print(t->params);
            printf(") ");

            break;

    }
}


void type_delete( struct type *t ) {
    if(!t) return;
    param_list_delete(t->params);
    type_delete(t->subtype);
    free(t);
}

bool type_equals( struct type *a, struct type *b )
{
    if( a->kind == b->kind ) {
        if ((TYPE_ARRAY == a->kind) && (TYPE_ARRAY == b-> kind)) {
            return type_equals(a->subtype, b->subtype);
        } else if ( a->kind == TYPE_FUNCTION && b->kind == TYPE_FUNCTION) {
            return type_equals(a->subtype, b->subtype) &&
            param_list_type_equals(a->params, b->params);
        } else return true;
    } else return false;
}

struct type * type_copy( struct type *t ) {
    if(!t) return NULL;
    struct type* copy = malloc(sizeof(*copy));
    copy->kind = t->kind;
    copy->params = param_list_copy(t->params);
    copy->subtype = type_copy(t->subtype);
    return copy;
}


