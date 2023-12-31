#include "io.h"
#include "buf.h"

#include <stdio.h>
#include <unistd.h>

/* TODO:
 */
void io_write(io_fd_t fd)
{
    (void) fd;
}

char io_read_char(io_fd_t fd)
{
    char ch;
    read(fd, &ch, 1);

    if (ch == '\n') {
        goto ret;
    }

    buf_t buf;
    buf_init(&buf, 8);
    io_read_until_newline(&buf, fd);

    buf_cleanup(&buf);

ret:
    return ch;
}

void io_read_until_newline(buf_t *buf, io_fd_t fd)
{
    int n_bytes = 0;

    while (buf_peek(buf) != '\n') {
        if (buf->ptr++ >= (int)buf->len) {
            buf_grow_size(buf);
        }

        n_bytes = read(fd, buf->content + buf->ptr, 1);

        if (n_bytes <= 0) {
            break;
        }
    }

    if (buf_peek(buf) == '\n') {
        buf_pop(buf);
    }

    buf_push_byte(buf, '\0');
}

bool io_read(buf_t *buf, io_fd_t fd)
{
    int n_bytes = 0;

    /* Worst case, the first read needs
     * to resize
     */
    if (buf->ptr + BUF_GROW_SIZE >= (int) buf->len) {
        buf_grow_size(buf);
    }

    while ((n_bytes = read(fd, buf->content + buf->ptr, BUF_GROW_SIZE)) > IO_EOF) {
        buf->ptr += n_bytes;

        if (buf->ptr + BUF_GROW_SIZE >= (int) buf->len) {
            buf_grow_size(buf);
        }
    }

    return n_bytes != -1;
}

bool io_close(io_fd_t fd)
{
    int ret = close(fd);
    return ret != -1;
}
