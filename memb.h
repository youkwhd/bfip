#ifndef BFCP_MEMB_H
#define BFCP_MEMB_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct memb_t {
    int *block;
    int ptr;
    size_t length;
} memb_t;

bool memb_init(memb_t *memb, size_t length);
void memb_cleanup(memb_t *memb);

#endif // BFCP_MEMB_H
