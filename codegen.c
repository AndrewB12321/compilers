#include "stmt.h"
#include "expr.h"
#include "decl.h"
#include "type.h"
#include "param_list.h"
#include "stack.h"
#include "codegen.h"
#include "symbol.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define table_size 7
char* registers[table_size] = {"%rbx","%r10","%r11","%r12","%r13","%r14","%r15" };
extern struct scratch_register** global_register_Table;
extern struct stack* global_stack;
extern FILE* global_output_file;
extern int global_label_count;

struct scratch_register** create_global_table() {
    struct scratch_register** table = malloc(sizeof(*table) * table_size);
    for(int i = 0; i < table_size; i++){
        struct scratch_register* tmp = table[i];
        tmp->name = registers[i];
        tmp->inuse = false;
    }
    return table;
}

int scratch_alloc() {
    for(int i = 0; i < table_size; i++){
        struct scratch_register* tmp = global_register_Table[i];
        if(!tmp->inuse) {
            tmp->inuse = true;
            return i;
        }
    }
    printf("Error: Out of scratch registers\n");
    exit(-1);
}

void scratch_free( int r ) {
    if(r >= table_size) return;
    struct scratch_register* tmp = global_register_Table[r];
    tmp->inuse = false;
}

const char * scratch_name( int r ) {
    if(r >= table_size) return 0;
    struct scratch_register* tmp = global_register_Table[r];
    return tmp->name;
}

int label_create() {
    /*
    post increment here makes it so the value returned is the 
    value before its incremented, so the first lable will be L0
    */
    return global_label_count++;
}

const char * label_name( int label ) {
    char l[10] = ".L";
    char n[5];
    sprintf(n, "%d", label);
    return strcat(l, n);
}

const char * symbol_codegen( struct symbol *s ) {
    switch(s->kind) {
        case SYMBOL_GLOBAL:
            return s->name;
        case SYMBOL_LOCAL:

        case SYMBOL_PARAM:
    }
}


void decl_codegen(struct decl *d) {
    if (!d)  return;
    switch (d->type->kind) {
        // Declaring local and global integers, as an example... (see p. 193)
        case TYPE_INTEGER:
            // If it's a local integer with an initializer...
            if (d->symbol->kind == SYMBOL_LOCAL && d->value != NULL) {
                expr_codegen(d->value);
                fprintf(global_output_file, "movq\t%s, %s\n", scratch_name(d->value->reg), symbol_codegen(d->symbol));
                scratch_free(d->value->reg);
            }
            // If it's a global integer...
            if (d->symbol->kind == SYMBOL_GLOBAL) {
                fprintf(global_output_file, ".data\n%s:\t.quad ", d->name);
                if (d->value) expr_print(d->value);
                else fprintf(global_output_file, "0\n");
            }
            break;
        // Declaring a function...
        case TYPE_FUNCTION:
            // Label using the function name
            fprintf(global_output_file, ".global %s\n", d->name);
            fprintf(global_output_file, "%s:\n", d->name);
            // Function prologue
            fprintf(global_output_file, "pushq %%rbp\n");
            fprintf(global_output_file, "movq %%rsp, %%rbp\n");
            int params_count = param_list_count(d->type->params);
            int locals_count = decl_local_count(d->code);
            fprintf(global_output_file, "subq $%d, %%rsp\n", 8 * (params_count + locals_count));
            const char *registers[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
            for (int i = 0; i < params_count; i++) {
                fprintf(global_output_file, "pushq %s\n", registers[i]);
            }
            // Function body
            stmt_codegen(d->code);
            // Function epilogue
            fprintf(global_output_file, "%s_epilogue:\n", d->name);
            fprintf(global_output_file, "movq %%rax, %%rdi\n");
            fprintf(global_output_file, "leave\n");
            fprintf(global_output_file, "ret\n");
            break;
    }
    decl_codegen(d->next);
}

void stmt_codegen(struct stmt* s) {

}

void expr_codegen(struct expr* e) {

}


void delete_global_table() {
    for(int i = 0; i < table_size; i++){
        free(global_register_Table[i]);
    }
}