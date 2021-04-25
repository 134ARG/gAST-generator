//
// Created by xen134 on 7/4/2021.
//

#ifndef CSCANNER_PARSER_GLOBALS_H
#define CSCANNER_PARSER_GLOBALS_H

#include "production.h"

// see parser_globals.c
extern stack matched_text;
extern stack symbol_names;
extern stack symbols;

void init_pglobals();
void destruct_pglobals();
void push_text(const char *str);
void clean_text();

symbol* get_symbol(const char *name);

void debug_print_symbol();
#endif //CSCANNER_SCANNER_GLOBALS_H
