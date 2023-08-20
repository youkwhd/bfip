#include <stdio.h>

#include "buf.h"
#include "repl.h"
#include "io.h"

#define REPL_BUF_INITIAL_SIZE 256

void repl_init(repl_t *repl, char *prompt)
{
    repl->prompt = prompt;
    buf_init(&repl->buf, REPL_BUF_INITIAL_SIZE);
}

char *repl_get_user_input(repl_t *repl)
{
    printf("%s ", repl->prompt);
    buf_reset(&repl->buf);
    io_read_until_newline(&repl->buf, IO_STDIN);
    return repl->buf.content;
}

void repl_cleanup(repl_t *repl)
{
    buf_cleanup(&repl->buf);
}
