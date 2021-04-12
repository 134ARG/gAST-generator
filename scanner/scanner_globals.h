//
// Created by xen134 on 29/3/2021.
// Global values and maintainer functions
//

#ifndef CSCANNER_SCANNER_GLOBALS_H
#define CSCANNER_SCANNER_GLOBALS_H

#define FUNCSIZE 5

#include "../lib/stack.h"

//extern int lineno;
//extern char *Text;
extern stack regex;
extern stack token_names;
extern char funcodes[][FUNCSIZE];

void init_globals();
void destruct_globals();

int get_funcode(const char *name);
size_t add_token_name(const char *name);
size_t get_token_code(const char *name);

#endif //CSCANNER_SCANNER_GLOBALS_H
