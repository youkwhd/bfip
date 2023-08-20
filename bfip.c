#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bfip.h"
#include "args.h"
#include "repl.h"
#include "buf.h"
#include "io.h"
#include "memb.h"

/* #define BFIP_DEBUG */

#define UNUSED(x) ((void) x)

#define UNIMPLEMENTED()

#define MEMB_INITIAL_SIZE 64
#define BUF_INITIAL_SIZE 256

/* Since brainfuck does not have any special
 * keywords, I think it is ok to skip lexical
 * analysis (for now).
 * 
 * Or, does interpreted languages does not need
 * lexical analysis?
 *
 * I wanted to add comments later on.
 */
int main(int argc, char **argv)
{
    args_t args;
    args_init(&args, argc, argv);

    memb_t memb;
    buf_t file_content_buf;

    if (!memb_init(&memb, MEMB_INITIAL_SIZE)) {
        exit(69);
    }

    /* TODO:
     * simplify main()
     */

    /* when -e script specified
     */
    if (args.script != NULL) {
        bfip_execute(&memb, args.script);
        return 0;
    }

    /* when user wants to use repl
     */
    if (args.use_repl) {
        repl_t repl;
        repl_init(&repl, "bfip>");

        char *input = "";

        while (strcmp(input, ":exit") != 0) {
            input = repl_get_user_input(&repl);
            bfip_execute(&memb, input);
        }

        memb_cleanup(&memb);
        repl_cleanup(&repl);
        return 0;
    }

    if (!buf_init(&file_content_buf, BUF_INITIAL_SIZE)) {
        memb_cleanup(&memb);
        exit(96);
    }

    /* TODO: 
     * Calling io_open() like this sucks
     */
    io_fd_t fd = io_open(args.file, O_RDONLY);

    if (!io_read(&file_content_buf, fd)) {
        memb_cleanup(&memb);
        buf_cleanup(&file_content_buf);
        io_close(fd);
        exit(17);
    }

    io_close(fd);

    /* TODO: 
     * Execute Brainfuck without buffering
     */
    bfip_execute(&memb, file_content_buf.content);

    memb_cleanup(&memb);
    buf_cleanup(&file_content_buf);
    return 0;
}

void bfip_execute(memb_t *memb, char *bf)
{
    for (char *ptr = bf; *ptr; ptr++) {
        switch (*ptr) {
        case '>':
            memb->ptr++;
            break;
        case '<':
            memb->ptr--;

            if (memb->ptr <= -1) {
                fprintf(stderr, "ERROR: memory pointer out of bounds (%d) \n", memb->ptr);
                exit(EXIT_FAILURE);
            }

            break;
        case '+':
            memb->block[memb->ptr]++;
            break;
        case '-':
            memb->block[memb->ptr]--;
            break;
        case '.':
            putchar(memb->block[memb->ptr]);
            break;
        case ',':
            memb->block[memb->ptr] = io_read_char(IO_STDIN);
            break;
        case '[': {
            int distance = 0;
            int skip = 0;

            for (char *tmp = ptr + 1; *tmp && skip != -1; tmp++) {
                distance++;
                skip += (*tmp == '[');
                skip -= (*tmp == ']');
            }

            if (skip != -1) {
                fprintf(stderr, "ERROR: expected ], not found\n");
                exit(EXIT_FAILURE);
            }

            if (memb->block[memb->ptr] == 0) {
                ptr += distance;
            }

            break;
        }
        case ']': {
            /* edge case when ]
             * is the first token
             */
            if (ptr == bf) {
                goto notfound;
            }

            int distance = 0;
            int skip = 0;

            for (char *tmp = ptr - 1; tmp != bf && skip != -1; tmp--) {
                distance--;
                skip += (*tmp == ']');
                skip -= (*tmp == '[');
            }

notfound:
            if (skip != -1) {
                fprintf(stderr, "ERROR: expected [, not found\n");
                exit(EXIT_FAILURE);
            }

            if (memb->block[memb->ptr] != 0) {
                ptr += distance;
            }

            break;
        }
        default:
            break;
        }
    }
}
