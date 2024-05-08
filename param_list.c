#include "stmt.h"
#include "expr.h"
#include "decl.h"
#include "type.h"
#include "symbol.h"
#include "param_list.h"
#include "stack.h"
#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern struct stack* global_stack;

struct param_list * param_list_create( char *name, struct type *type, struct param_list *next ) {
    struct param_list *tmp = malloc(sizeof(*tmp));
    tmp->name = name;
    tmp->type = type;
    tmp-> next = next;
    return tmp;
}

void param_list_print( struct param_list *a ) {
    if(!a) return;
    printf("%s:", a->name);
    type_print(a->type);
    if(a->next){
        printf(",");
        param_list_print(a->next);
    }
}

void param_list_resolve(struct param_list *a) {
    if(!a) return;
    a->symbol = symbol_create(SYMBOL_PARAM, a->type, a->name);
    scope_bind(a->name, a->symbol);
    param_list_resolve(a->next);

}

void param_list_delete( struct param_list *a ) {
    if(!a) return;
    type_delete(a->type);
    // symbol_delete(a->symbol); // not implemented yet
    param_list_delete(a->next);
    free(a); 

}

bool param_list_type_equals(struct param_list *a, struct param_list *b) {
    if(!a && !b) return true; 
    if((!a && b) || (a && !b)) return false; 
    return type_equals(a->type, b->type) && param_list_type_equals(a->next, b->next);
}

struct param_list * param_list_copy(struct param_list *p) {
    if(!p) return NULL;
    struct param_list* copy = malloc(sizeof(*copy));
    copy->name = p->name;
    copy->symbol = symbol_copy(p->symbol);
    copy->next = param_list_copy(p->next);
    return copy;
    
}