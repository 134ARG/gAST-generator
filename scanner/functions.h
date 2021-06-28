//
// Created by fort64 on 6/27/2021.
//

#ifndef GAST_GENERATOR_FUNCTIONS_H
#define GAST_GENERATOR_FUNCTIONS_H

#include "expression.h"
#include <stdlib.h>

int get_funcode(const char *name);
int apply(const char *str, size_t *i, expression *e);

#endif //GAST_GENERATOR_FUNCTIONS_H
