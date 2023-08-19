#ifndef BFIP_BUF_H
#define BFIP_BUF_H

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#define BUF_GROW_SIZE 256

typedef struct buf_t {
    char *content;
    size_t len;
    int ptr;
} buf_t;

bool buf_init(buf_t *buf, size_t len);
void buf_reset(buf_t *buf);
char buf_peek(buf_t *buf);
void buf_grow_size(buf_t *buf);
void buf_push_byte(buf_t *buf, char ch);
void buf_concat(buf_t *buf, char *str);
void buf_concatn(buf_t *buf, char *str, size_t len);
void buf_cleanup(buf_t *buf);

#endif // BFIP_BUF_H
