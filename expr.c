#include "expr.h"
#include <stdlib.h>

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right ) {
    expr* tmp = malloc(sizeof(*tmp));
    switch(kind) {
        
    }

}


struct expr * expr_create_name( const char *n ) {
    expr* tmp = malloc(sizeof(*tmp));
    tmp->name = name;
    return tmp;
}

struct expr * expr_create_integer_literal( int c ) {
    expr* tmp = malloc(sizeof(*tmp));
    tmp->literal_value = c;
    return tmp;
}

struct expr * expr_create_boolean_literal( int c ) {
    expr* tmp = malloc(sizeof(*tmp));
    tmp->literal_value = c;
    return tmp;
}

struct expr * expr_create_char_literal( char c ) {
    expr* tmp = malloc(sizeof(*tmp));
    tmp->sting_literal = c;
    return tmp;
}

struct expr * expr_create_string_literal( const char *str ) {
    expr* tmp = malloc(sizeof(*tmp));
    tmp->sting_literal = c;
    return tmp;
}