//
// Created by xen134 on 18/3/2021.
//

#ifndef SCANNER_SSTRING_H
#define SCANNER_SSTRING_H

#include <stddef.h>

struct sstring {
    size_t size;
    size_t length;
    char *data;
};

struct sstring *make_sstring(const char *str);
void init_sstring(struct sstring *p, const char *str);
int sspush(struct sstring *s, char ch);
char sspop(struct sstring *s);
char sstop(struct sstring *s);
char ssget(struct sstring *s, size_t index);
void ssdestruct(struct sstring *s);
void ssclean(struct sstring *s);
char *copy_to(struct sstring *s);

#endif //SCANNER_SSTRING_H
