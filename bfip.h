#ifndef BFIP_BFIP_H
#define BFIP_BFIP_H

#include "memb.h"

void bfip_execute(memb_t *memb, char *bf, void (*callback)(memb_t *memb, char *bf, int ip));

#endif // BFIP_BFIP_H
