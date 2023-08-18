#ifndef BFIP_MEMB_H
#define BFIP_MEMB_H

#include <stdint.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct memb_t {
    uint8_t *block;
    int ptr;
    size_t length;
} memb_t;

bool memb_init(memb_t *memb, size_t length);
void memb_cleanup(memb_t *memb);

#endif // BFIP_MEMB_H
