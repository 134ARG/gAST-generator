//
// Created by fort64 on 6/27/2021.
//

#include "functions.h"
#include "../lib/stack.h"
#include <string.h>

#define TRUE 1
#define FALSE 0
#define FUNCSIZE 5

static char funcodes[][FUNCSIZE] = {"cat", "or", "pls", "mul", "range"};

int cat(const char *str, size_t *i, struct stack *params);
int or(const char *str, size_t *i, struct stack *params);
int pls(const char *str, size_t *i, struct stack *params);
int mul(const char *str, size_t *i, struct stack *params);
int range(const char *str, size_t *i, struct stack *params);

// get function code by name
int get_funcode(const char *name) {
    for (int i = 0; i < FUNCSIZE; i++) {
        if (!strcmp(name, funcodes[i])) {
            return i;
        }
    }
    return -1;
}

// apply the input string to regex functions
int apply(const char *str, size_t *i, expression *e) {
    if (e->type == ATOM) {
        int res = !strncmp(str + *i, e->value.str, strlen(e->value.str));

        if (res) {
            *i += strlen(e->value.str);
            return TRUE;
        }
        return FALSE;
    }
    switch (e->funcode) {
        case 0: return cat(str, i, e->value.param);
        case 1: return or(str, i, e->value.param);
        case 2: return pls(str, i, e->value.param);
        case 3: return mul(str, i, e->value.param);
        case 4: return range(str, i, e->value.param);
        default: exit(1);
    }
}

// implementation of regex operation catenation
int cat(const char *str, size_t *i, struct stack *params) {
//    size_t max = strlen(str);

    for (int j = 0; j < params->length; j++) {
        struct expression *e = get(params, j);
        int res = apply(str, i, e);
        if (!res) return FALSE;
    }
    return TRUE;
}

// implementation of regex operation alternation
int or(const char *str, size_t *i, struct stack *params) {
    int res = 0;
    for (int j = 0; j < params->length; j++) {
        struct expression *e = get(params, j);
        res = apply(str, i, e) || res;
        if (res) return TRUE;
    }
    return FALSE;
}

// implementation of regex operation +
int pls(const char *str, size_t *i, struct stack *params) {
    struct expression *e = get(params, 0);
    int res = apply(str, i, e);
    if (!res) return FALSE;
    while (apply(str, i, e)) {}
    return TRUE;
}

// implementation for regex operation Kleene closure
int mul(const char *str, size_t *i, struct stack *params) {
    struct expression *e = get(params, 0);
    while (apply(str, i, e)) {}
    return TRUE;
}

// implementation of the range (-) operator
int range(const char *str, size_t *i, struct stack *params) {
    if (params->length < 2) exit(1);
    struct expression *start = get(params, 0);
    struct expression *end = get(params, 1);
    if (start->type != ATOM || end->type != ATOM) exit(1);
    if (*(str + *i) >= start->value.str[0] &&
        *(str + *i) <= end->value.str[0]) {
        (*i)++;
        return TRUE;
    }
    return FALSE;
}
