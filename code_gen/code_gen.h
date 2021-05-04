//
// Created by xen134 on 2/5/2021.
//

#ifndef GAST_GENERATOR_CODE_GEN_H
#define GAST_GENERATOR_CODE_GEN_H

#include "../lib/stack.h"
#include "../parser/AST.h"

ast_node *exp_to_prefix(ast_node *exp);
ast_node *declration_reassigner(ast_node *n);
ast_node *statement_reassigner(ast_node *n);
void show_prefix(stack *s);
ast_node *flatten(ast_node *exp);
void show_leaves(ast_node *s);
int is_nonterminal(ast_node *p, const char *str);

#endif //GAST_GENERATOR_CODE_GEN_H
