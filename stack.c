#include "stack.h"
#include <stdlib.h>
#include <string.h>

ValueStack *create_stack(int capacity){
    ValueStack *stack = malloc(sizeof(ValueStack));
    stack->items = malloc(sizeof(Value) * capacity);
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

void push(ValueStack *stack, Value value){
    if(stack->top + 1 >= stack->capacity){
        stack->capacity *= 2;
        stack->items = realloc(stack->items, sizeof(Value) * stack->capacity);
    }
    stack->items[++stack->top] = value;
}

Value pop(ValueStack *stack){
    if(stack->top < 0){
        exit(1);
    }
    return stack->items[stack->top--];
}

int is_empty(ValueStack *stack){
    return stack->top < 0;
}

void free_stack(ValueStack *stack){
    for(int i = 0; i <= stack->top; i++){
        if(stack->items[i].type == TYPE_STRING){
            free(stack->items[i].data.s);
        }
    }
    free(stack->items);
    free(stack);
}