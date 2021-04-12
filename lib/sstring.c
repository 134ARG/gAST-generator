//
// Created by xen134 on 18/3/2021.
//

//
// Created by xen134 on 2/23/21.
//

#include <malloc.h>
#include <memory.h>

#include "sstring.h"

static const size_t initial_size = 20;

// expand stack
static void enlarge(struct sstring *s) {
    void *p = malloc(s->size*4/3*sizeof(void *));
    if (p != NULL && p == memcpy(p, s->data, s->length*sizeof(void *))) {
        free(s->data);
        s->data = p;
        s->size = s->size * 4 / 3;
    }
}

// shrink stack
static void shrink(struct sstring *s) {
    size_t new_size = s->size*3/4;
    void *p = malloc(new_size*sizeof(char));
    if (p != NULL && p == memcpy(p, s->data, (s->length+1)*sizeof(char))) {
        free(s->data);
        s->data = p;
        s->size = new_size;
    }
}
struct sstring *make_sstring(const char *str) {
    struct sstring *p = malloc(sizeof(struct sstring));
    init_sstring(p, str);
    return p;
}
// initialize stack (stack must be initialized)
void init_sstring(struct sstring *p, const char *str) {
    p->size = initial_size;
    if (str != NULL) {
        p->length = strlen(str);
        p->data = malloc((p->length+1)*sizeof(char));
        strcpy(p->data, str);
    } else {
        p->length = 0;
        p->data = malloc(initial_size*sizeof(char));
        p->data[0] = '\0';
    }
}

// stack operation: push
int sspush(struct sstring *s, char ch) {
    if (s->length >= s->size) {
        enlarge(s);
    }
    if (s->length < s->size) {
        s->data[s->length] = ch;
        s->data[s->length+1] = '\0';
        s->length++;
        return 0;
    } else {
        return -1;
    }
}

// stack operation: pop
char sspop(struct sstring *s) {
    if (s->length > 0) {
        if (s->length < s->size / 2) {
            shrink(s);
        }
        char ch = s->data[--(s->length)];
        s->data[s->length] = '\0';
        return ch;
    } else {
        return '\0';
    }
}

// stack operation: top. Return the top element but keep it in the stack
char sstop(struct sstring *s) {
    if (s->length > 0) {
        return s->data[s->length - 1];
    }
    return '\0';
}

// stack(vector) operation: get. Random access
char ssget(struct sstring *s, size_t index) {
    if (s->length > index) {
        return s->data[index];
    } else {
        return '\0';
    }
}

char *copy_to(struct sstring *s) {
    char *p = malloc(sizeof(char)*(s->length+1));
    strcpy(p, s->data);
    return p;
}

// clean the content of the stack. After invocation the stack is unusable
void ssdestruct(struct sstring *s) {
    free(s->data);
    s->size = 0;
    s->length = 0;
}

// clean the content of the stack but keep it usable
void ssclean(struct sstring *s) {
    ssdestruct(s);
    init_sstring(s, NULL);
}


