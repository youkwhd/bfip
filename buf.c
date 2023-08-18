#include "buf.h"

#include <stdint.h>
#include <stdlib.h>

#define BUF_GROW_SIZE 256

bool buf_init(buf_t *buf, size_t len)
{
    buf->content = malloc((sizeof *buf->content) * len);
    buf->len = len;
    buf->ptr = -1;
    buf_push_byte(buf, '\0');
    return buf->content != NULL;
}

char buf_peek(buf_t *buf)
{
    return buf->content[buf->ptr];
}

void buf_grow_size(buf_t *buf)
{
    buf->content = realloc(buf->content, buf->len + BUF_GROW_SIZE);
}

void buf_push_byte(buf_t *buf, char ch)
{
    if (buf->ptr + 1 == (int)buf->len) {
        buf_grow_size(buf);
    }

    buf->content[++buf->ptr] = ch;
}

void buf_concat(buf_t *buf, char *str)
{
    if (buf_peek(buf) == '\0') {
        buf->ptr--;
    }

    while (*str) {
        buf_push_byte(buf, *str++);
    }

    if (buf_peek(buf) != '\0') {
        buf_push_byte(buf, '\0');
    }
}

void buf_concatn(buf_t *buf, char *str, size_t len)
{
    if (buf_peek(buf) == '\0') {
        buf->ptr--;
    }

    for (size_t i = 0; i < len; i++) {
        buf_push_byte(buf, str[i]);
    }

    if (buf_peek(buf) != '\0') {
        buf_push_byte(buf, '\0');
    }
}

void buf_cleanup(buf_t *buf)
{
    if (buf->content == NULL) {
        return;
    }

    free(buf->content);
}
