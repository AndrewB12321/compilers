#ifndef STACK_H
#define STACK_H

#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

struct stack {
    void** elems;
    size_t size;
    size_t capacity; 
};

struct stack* stack_create(); // creates an empty stack, default capacity 20
void   stack_push(struct stack* stk, void* item); // pushes a new element to the top of the stack
void*  stack_pop(struct stack* stk); // removes the top element and returns a pointer to that element
size_t stack_size(struct stack* stk); 
void*  stack_item(struct stack* stk, size_t position);
void   stack_delete(struct stack* stk);

#endif