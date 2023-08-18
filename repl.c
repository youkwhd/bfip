#include <stdio.h>

#include "repl.h"
#include "io.h"

void repl_get_user_input(repl_t *repl)
{
    printf("%s ", repl->prompt);
}
