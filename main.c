//
// Created by xen134 on 8/4/2021.
//

#include "parser/parser_globals.h"
#include "parser/sparser.h"
#include "parser/parser_main.h"
#include "scanner/scanner_globals.h"
#include "scanner/sparser.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv) {
    char *token_definition = NULL;
    char *nonterminal_definition = NULL;
    char *source_file = NULL;
    int debug_flag = 0;

    // read CMD options
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-t")) {
            token_definition = argv[++i];
        } else if (!strcmp(argv[i], "-s")) {
            source_file = argv[++i];
        } else if (!strcmp(argv[i], "-n")) {
            nonterminal_definition = argv[++i];
        } else if (!strcmp(argv[i], "--debug")) {
            debug_flag = 1;
        }
    }
    if (!token_definition) token_definition = "./micro_c.tokens";
    if (!nonterminal_definition) nonterminal_definition = "./micro_c.nonterminals";
    if (!source_file) {
        fprintf(stderr, "No input file specified.\n");
        exit(1);
    }

    init_globals();
    sparse_main(token_definition);

    p_sparse_main(nonterminal_definition);
    if (debug_flag) debug_print_symbol();

    parser_main(source_file);

    destruct_pglobals();
    destruct_globals();

    return 0;
}