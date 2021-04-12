//
// Created by xen134 on 28/3/2021.
//

#include "spredicate.h"
#include "expression.h"
#include "../lib/stack.h"
#include "string.h"

#define TRUE 1
#define FALSE 0

extern struct stack regex;
//extern struct stack token_names;

int cat(const char *str, size_t *i, struct stack *params);
int or(const char *str, size_t *i, struct stack *params);
int pls(const char *str, size_t *i, struct stack *params);
int mul(const char *str, size_t *i, struct stack *params);
int range(const char *str, size_t *i, struct stack *params);

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

// main predicate for deciding the matching token
int languagep(const char *str, size_t *init_index) {
    size_t max_len = 0;
    int max_index = -1;
    for (int i = 0; i < regex.length; i++) {
        size_t index = *init_index;
        int res = apply(str, &index, get(&regex, i));
//        printf("bias:%d\n", index);
        if (res && index - *init_index > max_len) {
            max_len = index - *init_index;
            max_index = i;
        }
    }
    if  (max_len) {
        *init_index += max_len;
        return max_index;
    } else {
        return -1;
    }
}