//
// Created by xen134 on 29/4/2021.
//

#include "error_report.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void general_error(const char *description, int lineno, const char *fmt, va_list valist) {
    printf("lineno: %d: %s:\n", lineno, description);
    fprintf(stderr, "lineno: %d: %s:\n", lineno, description);
    printf(fmt, valist);
    fprintf(stderr, fmt, valist);
    exit(1);
}
