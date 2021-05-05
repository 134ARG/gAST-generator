//
// Created by xen134 on 12/4/2021.
//

#include "parser_main.h"
#include "production.h"
#include "parser_globals.h"
#include "../scanner/scanner_main.h"
#include "../scanner/scanner_globals.h"
#include "../lib/sscanner.h"
#include "../lib/error_report.h"
#include "AST.h"
#include "match.h"
#include <stdio.h>
#include <string.h>

#include "../code_gen/code_gen.h"


static void parse_error(const char *fmt, ...) {
    va_list valist;
    va_start(valist, fmt);
    general_error("Error while parsing", get_lineno(), fmt, valist);
    va_end(valist);
}

// function for printing the final parsing tree recursively
void print_tree(ast_node *tree, int nest) {
    if (tree->type == TMP_VAL) {
        printf("%*c", 2 * nest, ' ');
        printf("|-> NULL for tmp slot\n");
        return;
    } else
    if (is_token(tree)) {
        printf("%*c", 2*nest, ' ');
        printf("|-> terminal: %s", get_token_name(tree->code));
        printf(": %s\n", tree->s);
        return;
    } else {
        symbol *s = get_symbol_by_code(tree->code);
        if (!s || s->type != TMP) {
//            if (is_nonterminal(tree, "exp")) {
//                ast_node *l = flatten(tree);
//                show_leaves(l);
//                copy_node(exp_to_prefix(l), tree);
//                printf("\n");
            /*} else*/ if (is_nonterminal(tree, "var_declarations")) {
                copy_node(declration_reassigner(flatten(tree)), tree);
            } else if (is_nonterminal(tree, "if_statement")) {
                ast_node *l = statement_reassigner(flatten(tree));
                l->type = IF;
                l->s = strdup("IF");
                //show_leaves(l);
                copy_node(l, tree);
            } else if (is_nonterminal(tree, "while_statement")) {
                ast_node *l = statement_reassigner(flatten(tree));
                l->type = WHILE;
                l->s = strdup("WHILE");
                //show_leaves(l);
                copy_node(l, tree);
            } else if (is_nonterminal(tree, "do_while_statement")) {
                ast_node *l = statement_reassigner(flatten(tree));
                l->type = DO;
                l->s = strdup("DO");
                //show_leaves(l);
                copy_node(l, tree);
            } else if (is_nonterminal(tree, "return_statement")) {
                ast_node *l = statement_reassigner(flatten(tree));
                l->type = RETURN;
                l->s = strdup("RETURN");
                //show_leaves(l);
                copy_node(l, tree);
            }
//            else if (is_nonterminal(tree, "exp")) {
//                ast_node *l = flatten(tree);
////                show_leaves(l);
//                copy_node(exp_to_prefix(l), tree);
//            }

            printf("%*c", 2 * nest, ' ');
            printf("|-> non-terminal: %s, %s, %d\n", get_symbol_name(tree->code), tree->s, tree->type);
            nest++;
        }

        for (int i = 0; i < tree->expr->length; i++) {
            ast_node *current = get(tree->expr, i);
            print_tree(current, nest);
        }
        if (is_nonterminal(tree, "code_block") || is_nonterminal(tree, "statement_block")) {
            ast_node *l = statement_reassigner(flatten(tree));
            l->type = STATEMENT;
            l->s = strdup("STATEMENT");
            //show_leaves(l);
            copy_node(l, tree);
        }
    }
}

void print_tree2(ast_node *tree, int nest) {
    if (tree->type == TMP_VAL) {
        printf("%*c", 2 * nest, ' ');
        printf("|-> NULL for tmp slot\n");
    } else
    if (is_token(tree)) {
        printf("%*c", 2*nest, ' ');
        printf("|-> terminal: %s", get_token_name(tree->code));
        printf(": %s\n", tree->s);
        return;
    } else {
        symbol *s = get_symbol_by_code(tree->code);
        if (!s || s->type != TMP) {
            if (is_nonterminal(tree, "exp")) {
                ast_node *l = flatten(tree);
                show_leaves(l);
                copy_node(exp_to_prefix(l), tree);
                tree->type = ARI;
                tree->s = strdup("ARI");
                printf("\n");
            }
            printf("%*c", 2 * nest, ' ');
            printf("|-> non-terminal: %s, %s, %d\n", get_symbol_name(tree->code), tree->s, tree->type);
            nest++;
        }

        for (int i = 0; i < tree->expr->length; i++) {
            ast_node *current = get(tree->expr, i);
            print_tree2(current, nest);
        }

    }
}

void print_tree3(ast_node *tree, int nest) {
    if (tree->type == TMP_VAL) {
        printf("%*c", 2 * nest, ' ');
        printf("|-> NULL for tmp slot\n");
    } else
    if (is_token(tree)) {
        printf("%*c", 2*nest, ' ');
        printf("|-> terminal: %s", get_token_name(tree->code));
        printf(": %s, %d\n", tree->s, tree->type);
        return;
    } else {
        symbol *s = get_symbol_by_code(tree->code);
        if (!s || s->type != TMP) {
            printf("%*c", 2 * nest, ' ');
            printf("|-> non-terminal: %s, %s, %d\n", get_symbol_name(tree->code), tree->s, tree->type);
            nest++;
        }

        for (int i = 0; i < tree->expr->length; i++) {
            ast_node *current = get(tree->expr, i);
            print_tree3(current, nest);
        }

    }
}

// main procedure for parsing source
void parser_main(const char *path) {
    init_scan(path);

    int token = next_token_s();

    // read first nonterminal as starting point
    symbol *general = get_symbol_by_code(0);
    ast_node  *tree = recursive_apply(general, &token);
    if (tree) {
        if (tree->type != EMPTY) {
            printf("----\n");
            print_tree2(tree, 0);
            print_tree(tree, 0);
            printf("--------------\n");
            print_tree3(tree, 0);

            gen_symbol_list(tree);
            debug_print_variables();
        }
    }


    if (token != -1) {
        parse_error("Unmatched token remains.\n");
    }

    destruct_tree(tree);
}