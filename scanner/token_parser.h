//
// Created by xen134 on 28/3/2021.
//

#ifndef CSCANNER_SCANNER_SPARSER_H
#define CSCANNER_SCANNER_SPARSER_H

#include "../lib/stack.h"
#include <stdlib.h>

struct token_context {
    stack* token_definitions;
    stack* token_names;
    stack* nonterminals;
    stack* nonterminal_names;
    u_int32_t id;
};

struct token_context* new_context();

//struct token_context* parse_script();
struct token_context* read_token_definitions(const char *path);

#endif //CSCANNER_SCANNER_SPARSER_H
