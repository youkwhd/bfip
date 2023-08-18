#include <stdio.h>

#define MEMB_INITIAL_SIZE 256

#include "memb.h"

int main(void)
{
    memb_t memb;
    memb_init(&memb, MEMB_INITIAL_SIZE);

    memb_cleanup(&memb);
    return 0;
}
