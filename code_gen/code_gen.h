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
int is_terminal(ast_node *p, const char *str);

void gen_exp(ast_node *ari);
stack *gen_symbol_list(ast_node *tree);
void debug_print_variables();
//void gen_statement(ast_node *statements);
void gen_statements(ast_node *tree);
void gen_main_epilogue();
void gen_main_prologue();
void copy_write();
void copy_read();
void add_extern();
void code_gen_main(ast_node *tree, const char *output);

#endif //GAST_GENERATOR_CODE_GEN_H
