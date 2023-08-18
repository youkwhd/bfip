#include <stdio.h>

#include "memb.h"

#define MEMB_INITIAL_SIZE 256

int main(int argc, char **argv)
{
    memb_t memb;
    memb_init(&memb, MEMB_INITIAL_SIZE);

    memb_cleanup(&memb);
    return 0;
}
