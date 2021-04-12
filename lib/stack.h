//
// Created by xen134 on 2/23/21.
//

#ifndef DFA_GENERATOR_STACK_H
#define DFA_GENERATOR_STACK_H

#include <stddef.h>

// data structure: stack. Only store void pointer.
typedef struct stack {
    size_t size;
    size_t length;
    void **data;
} stack;

void init_stack(struct stack *p);
int push(struct stack *s, void *p);
void *pop(struct stack *s);
void *top(struct stack *s);
void *get(struct stack *s, size_t index);
void copy_stack(stack *s, stack *d, size_t start, size_t end);
void destruct(struct stack *s);
int modify(struct stack *s, size_t index, void *new_value);
void clean(struct stack *s);
void free_content(struct stack *s);
struct stack *make_stack();

#endif //DFA_GENERATOR_STACK_H
