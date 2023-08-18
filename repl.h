#ifndef BFIP_REPL_H
#define BFIP_REPL_H

#include "buf.h"

typedef struct repl_t {
    char *prompt;
    buf_t buf;
} repl_t;

void repl_get_user_input(repl_t *repl);

#endif // BFIP_REPL_H
