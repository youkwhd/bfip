#include "memb.h"

#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

bool memb_init(memb_t *memb, size_t length)
{
    memb->block = malloc((sizeof *memb->block) * length);
    memset(memb->block, 0, length);
    memb->length = length;
    memb->ptr = 0;
    return memb->block != NULL;
}

void memb_cleanup(memb_t *memb)
{
    if (memb->block == NULL) {
        return;
    }

    free(memb->block);
}
