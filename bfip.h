#ifndef BFIP_BFIP_H
#define BFIP_BFIP_H

#include "memb.h"

void bfip_execute(memb_t *memb, char *program, void (*callback_fn_before)(memb_t *memb, char *program, int ip), void (*callback_fn_after)(memb_t *memb, char *program, int ip));

#endif // BFIP_BFIP_H
