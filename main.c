//
// Created by xen134 on 8/4/2021.
//

#include "parser/parser_globals.h"
#include "parser/sparser.h"
#include "parser/parser_main.h"
#include "scanner/tokens.h"
#include "scanner/token_parser.h"
#include "code_gen/code_gen.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv) {
    char *token_definition = NULL;
    char *nonterminal_definition = NULL;
    char *source_file = NULL;
    char *output_file = NULL;
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
        } else if (!strcmp(argv[i], "-o")) {
            output_file = argv[++i];
        }
    }
    if (!token_definition) token_definition = "./micro_c.tokens";
    if (!nonterminal_definition) nonterminal_definition = "./micro_c.nonterminals";
    if (!source_file) {
        fprintf(stderr, "No input file specified.\n");
        exit(1);
    }

    init_globals();
    read_token_definitions(token_definition);

    p_sparse_main(nonterminal_definition);
    if (debug_flag) debug_print_symbol();

    code_gen_main(parser_main(source_file), output_file);

    destruct_pglobals();
    destruct_globals();

    return 0;
}