#include <stdio.h>
#include <stdlib.h>

#include "buf.h"
#include "io.h"
#include "memb.h"

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
    UNUSED(argc);
    UNUSED(argv);

    char *filename = "./examples/69.bf";

    memb_t memb;
    buf_t file_content_buf;

    if (!memb_init(&memb, MEMB_INITIAL_SIZE)) {
        exit(69);
    }

    if (!buf_init(&file_content_buf, BUF_INITIAL_SIZE)) {
        memb_cleanup(&memb);
        exit(96);
    }

    /* TODO: 
     * Calling io_open() like this sucks
     */
    io_fd_t fd = io_open(filename, O_RDONLY);

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
    for (char *ch = file_content_buf.content; *ch; ch++) {
        switch (*ch) {
        case ' ':
        case '\n':
            UNIMPLEMENTED();
            break;
        case '>':
            memb.ptr++;
            break;
        case '<':
            memb.ptr--;
            break;
        case '+':
            memb.block[memb.ptr]++;
            break;
        case '-':
            memb.block[memb.ptr]--;
            break;
        case '.':
            printf("%c", memb.block[memb.ptr]);
            break;
        case ',':
            UNIMPLEMENTED();
            break;
        case '[':
            UNIMPLEMENTED();
            break;
        case ']':
            UNIMPLEMENTED();
            break;
        default:
            fprintf(stderr, "ERROR: unknown token :: %d\n", *ch);
            exit(EXIT_FAILURE);
        }
    }

    memb_cleanup(&memb);
    buf_cleanup(&file_content_buf);
    return 0;
}
