//
// Created by xen134 on 2/23/21.
//

#include <malloc.h>
#include <memory.h>

#include "stack.h"

static const size_t initial_size = 20;

// expand stack
static void enlarge(struct stack *s) {
    void *p = malloc(s->size*4/3*sizeof(void *));
    if (p != NULL && p == memcpy(p, s->data, s->length*sizeof(void *))) {
        free(s->data);
        s->data = p;
        s->size = s->size * 4 / 3;
    }
}

// shrink stack
static void shrink(struct stack *s) {
    size_t new_size = s->size*3/4;
    void *p = malloc(new_size*sizeof(void *));
    if (p != NULL && p == memcpy(p, s->data, s->length*sizeof(void *))) {
        free(s->data);
        s->data = p;
        s->size = new_size;
    }
}

// initialize stack (stack must be initialized)
void init_stack(struct stack *p) {
    p->size = initial_size;
    p->length = 0;
    p->data = malloc(initial_size*sizeof(void *));
}

struct stack *make_stack() {
    struct stack *p = malloc(sizeof(struct stack));
    init_stack(p);
    return p;
}

// stack operation: push
int push(struct stack *s, void *p) {
    if (s->length >= s->size) {
        enlarge(s);
    }
    if (s->length < s->size) {
        s->data[s->length] = p;
        s->length++;
        return 0;
    } else {
        return -1;
    }
}

// stack operation: pop
void *pop(struct stack *s) {
    if (s->length > 0) {
        if (s->length < s->size / 2) {
            shrink(s);
        }
        return s->data[--(s->length)];
    } else {
        return NULL;
    }
}

// stack operation: top. Return the top element but keep it in the stack
void *top(struct stack *s) {
    if (s->length > 0) {
        return s->data[s->length - 1];
    }
    return NULL;
}

// stack(vector) operation: get. Random access
void *get(struct stack *s, size_t index) {
    if (s->length > index && index >= 0) {
        return s->data[index];
    } else {
        return NULL;
    }
}

void set(stack *s, size_t index, void *data) {
    if (index > s->length) return;
    s->data[index] = data;
}

void swap(stack *s, size_t id1, size_t id2) {
    void *tmp = get(s, id1);
    set(s, id1, get(s, id2));
    set(s, id2, tmp);
}

void copy_stack(stack *s, stack *d, size_t start, size_t end) {
    if (end < start || start > s->length) return;
    if (end > s->length) end = s->length;
    d->length = end - start;
    d->size = s->size;
    if (d->data) free(d->data);
    d->data = malloc(d->size * sizeof(void *));
    d->data = memcpy(d->data, s->data+start, d->length*sizeof(void *));
}

int cat_stack(stack *s, stack *d) {
    if (d == NULL || s == NULL) {
        printf("errorrrrrrrrrrrrrr!\n");
        return 1;
    }
    for (int i = 0; i < d->length; i++) {
        push(s, get(d, i));
    }
}

void free_content(struct stack *s) {
    for (int i = 0; i < s->length; i++) {
        free(s->data[i]);
    }
}
// clean the content of the stack. After invocation the stack is unusable
void destruct(struct stack *s) {
    free(s->data);
    s->size = 0;
    s->length = 0;
}

// clean the content of the stack but keep it usable
void clean(struct stack *s) {
	destruct(s);
	init_stack(s);
}

stack *filter(int (*predicate)(void *), stack *p) {
    stack *r = make_stack();
    for (int i = 0; i < p->length; i++) {
        if (predicate(get(p, i))) {
            push(r, get(p, i));
        }
    }
    return r;
}

stack *map(return_type (f)(void *, size_t), stack *p) {
    stack *r = make_stack();
    for (int i = 0; i < p->length; i++) {
        return_type res = f(get(p, i), i);
        if (res.flag)
            push(r, res.val);
    }
    return r;
}

void *find(return_type (*f)(void *, size_t), stack *p) {
    for (int i = 0; i < p->length; i++) {
        return_type res = f(get(p, i), i);
        if (res.flag) {
            return res.val;
        }
    }
}

void destruct_and_free(stack *s) {
    destruct(s);
    free(s);
}
