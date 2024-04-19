#include "stmt.h"
#include "expr.h"
#include "decl.h"
#include "type.h"
#include "param_list.h"
#include "indent.h"
#include <stdio.h>
#include <stdlib.h>

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

void stmt_print( struct stmt *s, int indent, int special_case ) {
    if(!s) return;

    if(!special_case) {
        print_indent(indent); //Manages ifs and fors cases
    }

    switch (s->kind) {
        case STMT_DECL:
            decl_print(s->decl, indent);
            break;

        case STMT_EXPR:
            expr_print(s->expr);
            printf(";\n");
            break;

        case STMT_IF_ELSE:
            printf("if (");
            expr_print(s->expr);
            printf(") ");
            stmt_print(s->body, indent, 1);
            if(s->else_body) {
                print_indent(indent);
                printf("else ");
                stmt_print(s->else_body, indent, 1);
            }
            break;

        case STMT_FOR:
            printf("for(");
            if(s->init_expr) {
                expr_print(s->init_expr);
            }
            printf(";");
            if(s->expr) {
                expr_print(s->expr);
            }
            printf(";");
            if(s->next_expr) {
                expr_print(s->next_expr);
            }
            printf(") ");
            stmt_print(s->body, indent, 1);
            break;

        case STMT_PRINT:
            printf("print ");
            expr_print(s->expr);
            printf(";\n");
            break;

        case STMT_RETURN:
            printf("return");
            if(s->expr) {
                printf(" ");
                expr_print(s->expr);
            }
            printf(";\n");
            break;

        case STMT_BLOCK:
            printf("{\n");
            stmt_print(s->body, indent+1, 0);
            print_indent(indent);
            printf("}\n");
            break;
    }

    if(s->next) {
        stmt_print(s->next, indent, 0);
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

