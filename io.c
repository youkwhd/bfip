#include "io.h"
#include "buf.h"

#include <unistd.h>

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
