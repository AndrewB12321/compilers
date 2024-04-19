#include "stmt.h"
#include "expr.h"
#include "decl.h"
#include "type.h"
#include "param_list.h"
#include "indent.h"
#include <stdio.h>
#include <stdlib.h>

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
    if(!a->next){
        printf(",");
        param_list_print(a->next);
    }
}

void param_list_delete( struct param_list *a ) {
    if(!a) return;
    type_delete(a->type);
    // symbol_delete(a->symbol); // not implemented yet
    param_list_delete(a->next);
    free(a); 

}