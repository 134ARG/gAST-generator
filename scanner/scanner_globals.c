//
// Created by xen134 on 29/3/2021.
//

#include <string.h>
#include "../lib/stack.h"
#include "expression.h"
#include "scanner_globals.h"
#include "../lib/ctool.h"

// for sparser
stack regex;
stack token_names;
char funcodes[][FUNCSIZE] = {"cat", "or", "pls", "mul", "range"};

// initialize global stacks
void init_globals() {
    init_stack(&regex);
    init_stack(&token_names);
}

// free global stacks
void destruct_globals() {
    for (int i = 0; i < regex.length; i++) {
        free_expression(get(&regex, i));
    }
    free_content(&token_names);
    destruct(&regex);
    destruct(&token_names);
}

// get function code by name
int get_funcode(const char *name) {
    for (int i = 0; i < FUNCSIZE; i++) {
        if (!strcmp(name, funcodes[i])) {
            return i;
        }
    }
    return -1;
}

size_t add_token(const char *name) {
    push(&token_names, copy_string(name));
    return token_names.length - 1;
}

size_t get_token_code(const char *name) {
    for (int i = 0; i < token_names.length; i++) {
        if (!strcmp(get(&token_names, i), name)) {
            return i;
        }
    }
    return -1;
}

const char *get_token_name(size_t code) {
    return get(&token_names, code);
}


