#ifndef STACK_H
#define STACK_H

#include "hashmap.h"

typedef struct {
    Value *items;
    int top;
    int capacity;
} ValueStack;

ValueStack *create_stack(int capacity);
void push(ValueStack *stack, Value value);
Value pop(ValueStack *stack);
int is_empty(ValueStack *stack);
void free_stack(ValueStack *stack);

#endif