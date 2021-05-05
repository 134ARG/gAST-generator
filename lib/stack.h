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

typedef struct return_type {
    void *val;
    int flag;
} return_type;

void init_stack(struct stack *p);
int push(struct stack *s, void *p);
void *pop(struct stack *s);
void *top(struct stack *s);
void *get(struct stack *s, size_t index);
void set(stack *s, size_t index, void *data);
void swap(stack *s, size_t id1, size_t id2);
void copy_stack(stack *s, stack *d, size_t start, size_t end);
int cat_stack(stack *s, stack *d);
void destruct(struct stack *s);
void destruct_and_free(stack *s);
void clean(struct stack *s);
void free_content(struct stack *s);
struct stack *make_stack();
stack *filter(int (*predicate)(void *), stack *p);
stack *map(return_type (*f)(void *, size_t), stack *p);
void *find(return_type (*f)(void *, size_t), stack *p);

#endif //DFA_GENERATOR_STACK_H
