#include "decl.h"
#include <stdlib.h>

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next ) {
    decl* tmp = malloc(sizeof(tmp*));
    tmp->name = name;
    tmp->type = type;
    tmp->value = value;
    tmp->code = code;
    tmp->decl = next;
    return tmp;
}