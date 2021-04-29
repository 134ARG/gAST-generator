//
// Created by xen134 on 28/3/2021.
// main scanner for source file
//

#include "scanner_main.h"
#include "spredicate.h"
#include "../lib/sscanner.h"
#include "scanner_globals.h"
#include "../lib/sstring.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

//extern struct stack token_names;
//extern int lineno;

char *text;

static void scan_error(const char *message) {
    printf("lineno: %d: Error while scanning source: %s\n", get_lineno(), message);
    fprintf(stderr, "lineno: %d: Error while scanning source: %s\n", get_lineno(), message);
}

// read one unit from input file
int read_unit(struct sstring *str) {
    char ch = next_char(0);
    while (!isblank(ch) && !iscntrl(ch) && ch != -2 && ch != -1) {
//        if (ch == '\\') {
//            ch = next_char(0);
//        }
        sspush(str, ch);
        ch = next_char(0);
    }
    next_char(1);
    return 0;
}

// return next unit of the input file. Refer to report for more info.
char *next_unit() {
    char ch = next_char(0);
    struct sstring str;

    if (ch == -2) {
        return NULL;
    } else if (isblank(ch) || iscntrl(ch) || ch == -1) {
        return next_unit();
    } else {
        init_sstring(&str, NULL);
        sspush(&str, ch);
        read_unit(&str);
        char *p = copy_to(&str);
        ssdestruct(&str);
        return p;
    }
}

// main routine for scanner
void scanner_main(const char *input, const char *output) {
    open_file(input);
    FILE *out_file = NULL;
    if (output) {
        out_file = fopen(output, "w");
    }
    char *s = next_unit();
    while(s != NULL) {
        size_t index = 0;
        while (index < strlen(s)) {
            size_t prev = index;
            int token_code = languagep(s, &index);  // find token id
            if (token_code != -1) {
                const char *token = get_token_name(token_code);
                if (out_file)
                    fprintf(out_file, "%.*s: %s\n", (int) (index - prev), s + prev, token);
                else
                    printf("%.*s: %s\n", (int) (index - prev), s + prev, token);
            } else {    // token not found
                scan_error(s);
                scan_error("Such string doesn't match any pattern.");
                exit(1);
            }

        }
        free(s);
        s = next_unit();
    }
    if (out_file) fclose(out_file);
    clean_scan();
}

void str_copy(char *dest, char *src, size_t begin, size_t end) {
    for (int i = begin; i < end; i++) {
        dest[i-begin] = src[i];
        //printf("%c", src[i]);
    }
    //printf("end\n");

    dest[end - begin] = '\0';
}

// main routine for scanner
int next_token_s() {
    static size_t index = 0;
    static size_t prev = 0;
    static char *s = NULL;

    if (!s) s = next_unit();
    if (s && index >= strlen(s)) {
        free(s);
        index = prev = 0;
        s = next_unit();
    }
    if (!s) return -1;

    prev = index;
    int token_code = languagep(s, &index);  // find token id
    if (token_code != -1) {
        if (text) free(text);
        text = malloc(sizeof(char) * (index - prev + 1));
        str_copy(text, s, prev, index);
        return token_code;
    } else {    // token not found
        scan_error(s);
        scan_error("Such string doesn't match any pattern.");
        exit(1);
    }
}

void init_scan(const char *path) {
    open_file(path);
    text = NULL;
}