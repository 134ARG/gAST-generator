#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sparser.h"
#include "../lib/stack.h"
#include "scanner_main.h"
#include "scanner_globals.h"

//extern struct stack regex;
//extern struct stack token_names;

//int main(int argc, char **argv) {
//    int a = -1;
//    printf("%d\n", a >> 1);
//    char *script = NULL;
//    char *input_file = NULL;
//    char *output_file = NULL;
//
//    // read CMD options
//    for (int i = 1; i < argc; i++) {
//        if (!strcmp(argv[i], "-s")) {
//            script = argv[++i];
//        } else if (!strcmp(argv[i], "-i")) {
//            input_file = argv[++i];
//        } else if (!strcmp(argv[i], "-o")) {
//            output_file = argv[++i];
//        }
//    }
//    if (!script) script = "./micro_c.tokens";
//    if (!input_file) {
//        fprintf(stderr, "No input file specified.\n");
//        exit(1);
//    }
//
//    init_globals();
//
//    // load token definition
//    sparse_main(script);
//    // scan micro C source
//    scanner_main(input_file, output_file);
//    destruct_globals();
//    return 0;
//}
