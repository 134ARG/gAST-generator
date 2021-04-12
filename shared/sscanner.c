//
// Created by xen134 on 28/3/2021.
// file read functions for both the script and the source
//

#include "sscanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../lib/sstring.h"
#include "ctool.h"

static FILE *input = NULL;
int lineno;
char *Text;

static void scan_error(const char *message) {
    printf("lineno: %d: Error while scanning: %s\n", lineno, message);
    fprintf(stderr, "lineno: %d: Error while scanning: %s\n", lineno, message);
}

// open input file
int open_file(const char *path) {
    input = fopen(path, "r");
    if (!input) {
        scan_error(path);
        scan_error("Can not open input file.");
        exit(1);
    }
    return 0;
}

// return next char once called. If fallback is true, move back one char.
char next_char(int fallback) {
    static int index = 0;
    static char *line = NULL;
    size_t buffer_size = 0;
    static int length = 0;

    if (fallback && index > 0) {
        index--;
        return '\0';
    }

    if (index == length) {
        if (line != NULL) {
            free(line);
            line = NULL;
        }

        length = getline(&line, &buffer_size, input);
        index = 0;

        if (length == -1) {
            if (lineno == 0) {
                scan_error("Can not read file.");
                exit(1);
            } else {
                index = 0;
                length = 0;
                return -2;
            }
        } else {
            lineno++;
        }
        if (lineno > 1){ return -1; }
    }
    char ch = line[index++];
    return ch;
}

// read one symbol from the input file
int read_symbol(struct sstring *str) {
    char ch = next_char(0);
    while (issymbol(ch)) {
        if (ch == '\\') {
            ch = next_char(0);
        }
        sspush(str, ch);
        ch = next_char(0);
    }
    next_char(1);
    return 0;
}

// return next token of the input file
enum token_type next_token() {
    char ch = next_char(0);
    struct sstring str;

    if (Text != NULL) {
        free(Text);
        Text = NULL;
    }

    if (ch == -2) {
        return EOSCAN;
    } else if (isblank(ch) || iscntrl(ch) || ch == -1) {
        return next_token();
    } else if (islparen(ch)) {
        return LPAREN;
    } else if (isrparen(ch)) {
        return RPAREN;
    } else if (iscolon(ch)) {
        return COLON;
    } else if (issemi(ch)) {
        return SEMICOLON;
    } else if (isbar(ch)) {
        return BAR;
    } else {
        init_sstring(&str, NULL);
//        sspush(&str, ch);
        next_char(1);
        read_symbol(&str);
        Text = copy_to(&str);
        ssdestruct(&str);
        return SYMBOL;
    }
}



// free memories
void clean_scan() {
    if (!Text) {
        free(Text);
        Text = NULL;
    }
    if (!input) {
        fclose(input);
        input = NULL;
    }
    lineno = 0;
}
