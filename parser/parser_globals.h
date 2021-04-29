//
// Created by xen134 on 7/4/2021.
//

#ifndef CSCANNER_PARSER_GLOBALS_H
#define CSCANNER_PARSER_GLOBALS_H

#include "production.h"

void init_pglobals();
void destruct_pglobals();
void push_text(const char *str);
void clean_text();

symbol* get_symbol(const char *name);
symbol *get_symbol_by_code(size_t code);
size_t symbols_length();
stack *symbols_stack();
const char *get_symbol_name(size_t code);

void debug_print_symbol();
#endif //CSCANNER_SCANNER_GLOBALS_H
