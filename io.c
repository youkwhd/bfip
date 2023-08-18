#include "io.h"
#include "buf.h"

#include <unistd.h>

bool io_read(buf_t *buf, io_fd_t fd)
{
    int n_bytes = 0;

    while ((n_bytes = read(fd, buf->content, buf->len)) > IO_EOF) {
        buf->ptr += n_bytes;

        if (buf->ptr >= buf->len) {
            buf_grow_sizen(buf, buf->len);
        }
    }

    return n_bytes != -1;
}

bool io_close(io_fd_t fd)
{
    int ret = close(fd);
    return ret != -1;
}
