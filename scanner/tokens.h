//
// Created by xen134 on 29/3/2021.
// Global values and maintainer functions
//

#ifndef CSCANNER_SCANNER_GLOBALS_H
#define CSCANNER_SCANNER_GLOBALS_H

#define FUNCSIZE 5

#include "../lib/stack.h"

void init_globals();
void destruct_globals();

int get_funcode(const char *name);
size_t add_token(const char *name);
size_t get_token_code(const char *name);
const char *get_token_name(size_t code);
stack *get_regex_stack();

#endif //CSCANNER_SCANNER_GLOBALS_H
