#include "stmt.h"
#include "expr.h"
#include "decl.h"
#include "symbol.h"
#include "type.h"
#include "param_list.h"
#include "codegen.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

extern struct stack* global_stack;

struct stmt * stmt_create( stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next ) {
    struct stmt *tmp = malloc(sizeof(*tmp));
    tmp->kind = kind;
    tmp->decl = decl;
    tmp->init_expr = init_expr;
    tmp->expr = expr;
    tmp->next_expr = next_expr;
    tmp->body = body;
    tmp->else_body = else_body;
    tmp->next = next;
    return tmp;

}

void stmt_print( struct stmt *s, int indent) {
    if(!s) return;
    switch (s->kind) {
        case STMT_DECL:
            print_indent(indent);
            decl_print(s->decl, indent);
            break;

        case STMT_EXPR:
            print_indent(indent);
            expr_print(s->expr);
            printf(";\n");
            break;

        case STMT_IF_ELSE:
            print_indent(indent);
            printf("if (");
            expr_print(s->expr);
            printf(") ");
            stmt_print(s->body, indent + 1);
            if(s->else_body) {
                print_indent(indent);
                printf (" else ");
                stmt_print(s->else_body, indent+1);
                printf("\n");
            }
            break;

        case STMT_FOR:
            print_indent(indent);
            printf("for(");
            expr_print(s->init_expr);
            printf(";");
            expr_print(s->expr);
            printf(";");
            expr_print(s->next_expr);
            printf(") ");
            stmt_print(s->body, indent+1);
            printf("\n");
            break;

        case STMT_PRINT:
            print_indent(indent);
            printf("print ");
            expr_print(s->expr);
            printf(";\n");
            break;

        case STMT_RETURN:
            print_indent(indent);
            printf("return");
            if(s->expr) {
                printf(" ");
                expr_print(s->expr);
            }
            printf(";\n");
            break;

        case STMT_BLOCK:
            printf(" {\n");
            print_indent(indent);
            stmt_print(s->body, indent+1);
            print_indent(indent);
            printf("}\n");
            break;
    }

    if(s->next) {
        stmt_print(s->next, indent);
    }

}

void stmt_resolve(struct stmt *s) {
    if(!s) return;
    switch(s->kind){
        case STMT_DECL:
            decl_resolve(s->decl);
            break;
        case STMT_EXPR:
        case STMT_PRINT:
        case STMT_RETURN:
            expr_resolve(s->expr);
            break;
        case STMT_IF_ELSE:
            expr_resolve(s->expr);
            stmt_resolve(s->body);
            stmt_resolve(s->else_body);
            break;
        case STMT_FOR:
            expr_resolve(s->expr);
            expr_resolve(s->expr);
            expr_resolve(s->expr);
            stmt_resolve(s->body);
            break;
            expr_resolve(s->expr);
            break;
        case STMT_BLOCK:
            scope_enter();
            stmt_resolve(s->body);
            scope_exit();
            break;
            
    }
}

void stmt_typecheck( struct stmt *s ) {
    struct type *t;
    switch(s->kind) {
        case STMT_PRINT:
        case STMT_RETURN:
        case STMT_EXPR:
            t = expr_typecheck(s->expr);
            type_delete(t);
            break;
        case STMT_IF_ELSE:
            t = expr_typecheck(s->expr);
            if(t->kind!=TYPE_BOOLEAN) {
            /* display an error */
            }
            type_delete(t);
            stmt_typecheck(s->body);
            stmt_typecheck(s->else_body);
            break;
        case STMT_DECL:
            decl_typecheck(s->decl);
            break;
        case STMT_FOR:
            t = expr_typecheck(s->init_expr);
            if(t->kind != TYPE_INTEGER) {
                // error
            }
            type_delete(t);
            t = expr_typecheck(s->expr);
            if(t->kind != TYPE_INTEGER) {
                // error
            }
            type_delete(t);
            t = expr_typecheck(s->next_expr);
            if(t->kind != TYPE_INTEGER) {
                // error
            }
            type_delete(t);
            break;
        case STMT_BLOCK:
            stmt_typecheck(s->body);
            break;
            
    /* more cases here */
    }
}


void stmt_delete( struct stmt *s ) {
    if(!s) return;
    decl_delete(s->decl);
    expr_delete(s->init_expr);
    expr_delete(s->expr);
    expr_delete(s->next_expr);
    stmt_delete(s->body);
    stmt_delete(s->else_body);
    stmt_delete(s->next);
    free(s);
}

void print_indent(int indent) {
    for(int i = 0; i < indent; i++) {
        printf(" ");
    }
}