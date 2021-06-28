//
// Created by xen134 on 28/3/2021.
// main scanner for source file
//

#include "scanner.h"
#include "../lib/lexer.h"
#include "tokens.h"
#include "../lib/sstring.h"
#include "../lib/error_report.h"
#include "functions.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>

char *text;

static void scan_error(const char *fmt, ...) {
    va_list valist;
    va_start(valist, fmt);
    general_error("Error while scanning source", get_lineno(), fmt, valist);
    va_end(valist);
}

// read one unit from input file
static int read_segment(struct sstring *str) {
    char ch = next_char(0);
    while (!isblank(ch) && !iscntrl(ch) && ch != -2 && ch != -1) {
        sspush(str, ch);
        ch = next_char(0);
    }
    next_char(1);
    return 0;
}

// return next unit of the input file. Refer to report for more info.
static char *next_segment() {
    char ch = next_char(0);
    struct sstring str;

    if (ch == -2) {
        return NULL;
    } else if (isblank(ch) || iscntrl(ch) || ch == -1) {
        return next_segment();
    } else {
        init_sstring(&str, NULL);
        sspush(&str, ch);
        read_segment(&str);
        char *p = copy_to(&str);
        ssdestruct(&str);
        return p;
    }
}

void str_copy(char *dest, const char *src, size_t begin, size_t end) {
    for (size_t i = begin; i < end; i++) {
        dest[i-begin] = src[i];
    }
    dest[end - begin] = '\0';
}

// main predicate for deciding the matching token
int languagep(const char *str, size_t *init_index) {
    size_t max_len = 0;
    int max_index = -1;
    for (int i = 0; i < get_regex_stack()->length; i++) {
        size_t index = *init_index;
        int res = apply(str, &index, get(get_regex_stack(), i));
        if (res && index - *init_index > max_len) {
            max_len = index - *init_index;
            max_index = i;
        }
    }
    if  (max_len) {
        *init_index += max_len;
        return max_index;
    } else {
        return -1;
    }
}

// main routine for scanner
int next_token() {
    static size_t index = 0;
    static size_t prev = 0;
    static char *s = NULL;

    if (!s) s = next_segment();
    if (s && index >= strlen(s)) {
        free(s);
        index = prev = 0;
        s = next_segment();
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
        scan_error("Such string doesn't match any pattern: %s.\n", s);
        exit(1);
    }
}

void init_scan(const char *path) {
    open_file(path);
    text = NULL;
}