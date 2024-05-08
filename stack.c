#include <stdio.h>
#include <stdlib.h>
#include "stack.h"


struct stack* stack_create() {
    struct stack* stk = malloc(sizeof(*stk));
    stk->elems = malloc(sizeof(void*)*20);
    stk->size = 0;
    stk->capacity = 20;
    return stk;
}

void   stack_push(struct stack* stk, void* item) {
    /*
        not sure if this copy works properly, but
        i dont think any program will have more than
        20 nested scopes so i think it will be fine
    */
    if(stk->size == stk->capacity){
        struct stack* tmp = malloc(sizeof(*stk));
        tmp->elems = malloc(sizeof(void*)*stk->capacity*2);
        tmp->size = stk->size;
        tmp->capacity = stk->capacity*2;
        for(int i = 0; i < stk->size; i++){
            tmp->elems[i] = stk->elems[i];
        }
        free(stk->elems);
        free(stk);
        stk = tmp;
    }
    stk->elems[stk->size] = item;
    stk->size++;
}

void*  stack_pop(struct stack* stk) {
    stk->size--;
    return stk->elems[stk->size];
}

size_t stack_size(struct stack* stk) {
    return stk->size;
}

void*  stack_item(struct stack* stk, size_t position) {
    if(position >= stk->size) return NULL;
    return stk->elems[position];
}
/*
    ************************IMPORTANT *****************************************
    since this stack has no way of telling what each element is and how to free it,
    it is up to the user to empty the stack and free each element before calling this 
    function, and it wont work unless the stack is empty
*/
void   stack_delete(struct stack* stk) {
    if(stk->size != 0) printf("stack is not empty\n");
    else {
        free(stk->elems);
        free(stk);
    }
}