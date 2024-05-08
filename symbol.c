#include "stmt.h"
#include "expr.h"
#include "decl.h"
#include "type.h"
#include "param_list.h"
#include "symbol.h"
#include "hash_table.h"
#include "codegen.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern struct stack* global_stack;
extern struct scratch_register* global_register_Table;
extern FILE* global_output_file;


struct symbol * symbol_create( symbol_t kind, struct type *type, char *name ) {
    struct symbol *s = malloc(sizeof(*s));
    s->kind = kind;
    s->type = type;
    s->name = name;
    return s;
}


void scope_enter() {
    struct hash_table* newTab = hash_table_create(0, 0);
    stack_push(global_stack, newTab);
}
void scope_exit() {
    struct hash_table* h = (struct hash_table*) stack_pop(global_stack);
    hash_table_delete(h);
}
int scope_level() {
    return stack_size(global_stack); 
}
void scope_bind(const char* name, struct symbol *sym) {
    struct hash_table* h = (struct hash_table*) stack_item(global_stack, stack_size(global_stack)-1);
    hash_table_insert(h, name, sym);
}
struct symbol *scope_lookup(const char *name) {
    struct symbol* sym = NULL;
    for (int i = (stack_size(global_stack) - 1); i >= 0; i--) {
        struct hash_table *ht = stack_item(global_stack, i);
        sym = (struct symbol*) hash_table_lookup(ht, name);
        if (sym) break;
        }
    return sym;
}
struct symbol *scope_lookup_current(const char *name) {
    struct hash_table *ht = stack_item(global_stack, stack_size(global_stack) - 1);
    struct symbol* sym = (struct symbol*) hash_table_lookup(ht, name);
    return sym;
}
struct symbol * symbol_copy( struct symbol *s ) {
    struct symbol *copy = malloc(sizeof(*copy));
    copy->kind = s->kind;
    copy->type = type_copy(s->type);
    copy->name = s->name;
    copy->which = s->which;
    return copy;
}

void symbol_delete(struct symbol* s) {
    if(!s) return;
    type_delete(s->type);
    free(s);
}
