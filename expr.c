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

extern struct stack* global_stack;
extern struct scratch_register* global_register_Table;
extern FILE* global_output_file;

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
            printf(" == ");
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
            printf(")");
            break;
        case EXPR_ARG:
            expr_print(e->left);
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

void expr_resolve(struct expr *e) {
    if(!e) return;
    if( e->kind==EXPR_NAME ) {
    e->symbol = scope_lookup(e->name);
    } else {
        expr_resolve( e->left );
        expr_resolve( e->right );
    }
}


void expr_delete( struct expr *e )
{
	/* Careful: Stop on null pointer. */
	if(!e) return;
	expr_delete(e->left);
	expr_delete(e->right);
    symbol_delete(e->symbol);
	free(e);
}

struct type * expr_typecheck( struct expr *e ) {
    if(!e) return 0;
    struct type *lt = expr_typecheck(e->left);
    struct type *rt = expr_typecheck(e->right);
    struct type *result;
    switch(e->kind) {
        case EXPR_UNARYSUB:
        case EXPR_ADD:
        case EXPR_SUB:
        case EXPR_MULT:
        case EXPR_DIV:
        case EXPR_MOD:
        case EXPR_EXP:
            if( lt->kind!=TYPE_INTEGER || rt->kind!=TYPE_INTEGER ) {
            /* display an error */
            }
            result = type_create(TYPE_INTEGER,0,0);
            break;
        case EXPR_ASSIGN:
            if(!type_equals(lt, rt)) {
                // error
            }
            result = type_create(rt->kind,0,0);
            break;
        case EXPR_OR:
        case EXPR_AND:
        case EXPR_LESSEQ:
        case EXPR_LESS:
        case EXPR_GREAT:
        case EXPR_EQ:
        case EXPR_NEQ:
        case EXPR_GREATEQ:
            if(!type_equals(lt,rt)) {
            /* display an error */
            }
            if(lt->kind==TYPE_VOID || lt->kind==TYPE_ARRAY || lt->kind==TYPE_FUNCTION) {
            /* display an error */
            }
        result = type_create(TYPE_BOOLEAN,0,0);
        break;
        case EXPR_NEGATE:
            if(lt->kind != TYPE_BOOLEAN ||lt->kind != TYPE_INTEGER) {
                // error
            }
            result = type_create(lt->kind,0,0);
            break;
        case EXPR_INC:
        case EXPR_DEC:
            if(lt->kind != TYPE_INTEGER) {
                // error
            }
            result = type_create(TYPE_INTEGER,0,0);
            break;
        case EXPR_NAME:
            result = type_create(TYPE_STRING,0,0);
            break;
        case EXPR_BOOL_L:
            result = type_create(TYPE_BOOLEAN,0,0);
            break;
        case EXPR_CHAR_L:
            result = type_create(TYPE_CHARACTER,0,0);
            break;
        case EXPR_NUM_L:
            result = type_create(TYPE_INTEGER,0,0);
            break;
        case EXPR_STRING_L:
            result = type_create(TYPE_STRING,0,0);
            break;
        case EXPR_CALL:
        case EXPR_ARG:
            break;
        case EXPR_SUBSCRIPT:
            if(lt->kind==TYPE_ARRAY) {
                if(rt->kind!=TYPE_INTEGER) {
                    /* error: index not an integer */
                }
                result = type_copy(lt->subtype);
            } else {
                /* error: not an array */
                /* but we need to return a valid type */
                result = type_copy(lt);
            }
        case EXPR_EXPR_LIST: 
            if(lt->kind==TYPE_ARRAY) {
                if(rt->kind!=TYPE_INTEGER) {
                    /* error: index not an integer */
                }
                result = type_copy(lt->subtype);
            } else {
                /* error: not an array */
                /* but we need to return a valid type */
                result = type_copy(lt);
            }
            break;  
    }
    type_delete(lt);
    type_delete(rt);
    return result;
}

