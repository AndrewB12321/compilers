#include "stmt.h"
#include "expr.h"
#include "decl.h"
#include "type.h"
#include "param_list.h"
#include "indent.h"
#include <stdio.h>
#include <stdlib.h>

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right ) {
    struct expr* tmp = malloc(sizeof(*tmp));
    tmp->kind = kind;
    tmp->left = left;
    tmp->right = right;
    return tmp;

}


struct expr * expr_create_name( const char *n ) {
    struct expr *e = expr_create(EXPR_NAME,0,0);
	e->name = n;
	return e;
}

struct expr * expr_create_integer_literal( int c ) {
    struct expr *e = expr_create(EXPR_NUM_L,0,0);
	e->literal_value = c;
	return e;
}

struct expr * expr_create_boolean_literal( int c ) {
    struct expr *e = expr_create(EXPR_BOOL_L,0,0);
	e->literal_value = c;
	return e;
}

struct expr * expr_create_char_literal( char c ) {
    struct expr *e = expr_create(EXPR_CHAR_L,0,0);
	e->literal_value = (int)c;
	return e;
}

struct expr * expr_create_string_literal( const char *str ) {
    struct expr *e = expr_create(EXPR_STRING_L,0,0);
	e->string_literal = str;
	return e;
}

void expr_print( struct expr *e ) {
    if(!e) return;
    switch(e->kind) {
        case EXPR_ADD:
            expr_print(e->left);
            printf(" + ");
            expr_print(e->right);
            break;
        case EXPR_SUB:
            expr_print(e->left);
            printf(" - ");
            expr_print(e->right);
            break;
        case EXPR_MULT:
            expr_print(e->left);
            printf(" * ");
            expr_print(e->right);
            break;
        case EXPR_DIV:
            expr_print(e->left);
            printf(" / ");
            expr_print(e->right);
            break;
        case EXPR_ASSIGN:
            expr_print(e->left);
            printf(" = ");
            expr_print(e->right);
            break;
        case EXPR_OR:
            expr_print(e->left);
            printf(" || ");
            expr_print(e->right);
            break;
        case EXPR_AND:
            expr_print(e->left);
            printf(" && ");
            expr_print(e->right);
            break;
        case EXPR_LESSEQ:
            expr_print(e->left);
            printf(" <= ");
            expr_print(e->right);
            break;
        case EXPR_LESS:
            expr_print(e->left);
            printf(" < ");
            expr_print(e->right);
            break;
        case EXPR_GREAT:
            expr_print(e->left);
            printf(" > ");
            expr_print(e->right);
            break;
        case EXPR_EQ: 
            expr_print(e->left);
            printf(" >= ");
            expr_print(e->right);
            break;
        case EXPR_NEQ:
            expr_print(e->left);
            printf(" != ");
            expr_print(e->right);
            break;
        case EXPR_GREATEQ:
            expr_print(e->left);
            printf(" + ");
            expr_print(e->right);
            break;
        case EXPR_MOD:
            expr_print(e->left);
            printf(" %% ");
            expr_print(e->right);
            break;
        case EXPR_EXP:
            expr_print(e->left);
            printf(" ^ ");
            expr_print(e->right);
            break;
        case EXPR_NEGATE:
            printf(" ! ");
            expr_print(e->left);
            break;
        case EXPR_UNARYSUB:
            printf(" -");
            expr_print(e->left);
            break;
        case EXPR_INC:
            expr_print(e->left);\
            printf("++ ");
            break;
        case EXPR_DEC:
            expr_print(e->left);\
            printf("-- ");
            break;
        case EXPR_NAME:
            printf("%s", e->name);
            break;
        case EXPR_BOOL_L:
            printf("%s", e->literal_value ? "true" : "false");
            break;
        case EXPR_CHAR_L:
            printf("%c", (char)e->literal_value);
            break;
        case EXPR_NUM_L:
            printf("%d", e->literal_value);
            break;
        case EXPR_STRING_L:
            printf("%s", e->string_literal);
            break;
        case EXPR_CALL:
            expr_print(e->left);
            printf("(");
            expr_print(e->right);
            break;
        case EXPR_ARG:
            expr_print(e->left);
            break;
            if(e->right){
                printf(", ");
                expr_print(e->right);
            }
            break;
        case EXPR_SUBSCRIPT:
            expr_print(e->left);
            printf("[");
            expr_print(e->right);
            printf("]");
            break;
        case EXPR_EXPR_LIST:
            expr_print(e->left);
            if(e->right){
                printf(", ");
                expr_print(e->right);
            }
            break;
    }


}

void expr_delete( struct expr *e )
{
	/* Careful: Stop on null pointer. */
	if(!e) return;
	expr_delete(e->left);
	expr_delete(e->right);
	free(e);
}
