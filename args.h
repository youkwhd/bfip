#ifndef BFIP_ARGS_H
#define BFIP_ARGS_H

#include <stdbool.h>

typedef struct args_t {
    char *file;
    char *script;
} args_t;

void args_init(args_t *args, int argc, char **argv);

#endif // BFIP_ARGS_H
