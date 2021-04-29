//
// Created by xen134 on 29/4/2021.
//

#ifndef GAST_GENERATOR_ERROR_REPORT_H
#define GAST_GENERATOR_ERROR_REPORT_H

#include <stdarg.h>

void general_error(const char *description, int lineno, const char *fmt, va_list valist);

#endif //GAST_GENERATOR_ERROR_REPORT_H
