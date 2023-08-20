#ifndef BFIP_IO_H
#define BFIP_IO_H

#include "buf.h"
#include <stdbool.h>
#include <fcntl.h>

#define IO_EOF 0

/* i am sorry */
#define io_open open

typedef int io_fd_t;

#define IO_STDIN 0
#define IO_STDOUT 1
#define IO_STDERR 2

char io_read_char(io_fd_t fd);
void io_read_until_newline(buf_t *buf, io_fd_t fd);
bool io_read(buf_t *buf, io_fd_t fd);
bool io_close(io_fd_t fd);

#endif // BFIP_IO_H
