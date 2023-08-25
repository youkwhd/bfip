#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bfip.h"
#include "args.h"
#include "buf.h"
#include "io.h"
#include "memb.h"

#define UNUSED(x) ((void) x)

#define MEMB_INITIAL_SIZE 64
#define BUF_INITIAL_SIZE 256

void bfip_debug_callback(memb_t *memb, char *bf, int ip)
{
    if (bf[ip] == '.') {
        putchar('\n');
    }

    printf("[DEBUG ::] INST (%d := %c) MEMORY (%d:%d)\n", ip, bf[ip], memb->ptr, memb->block[memb->ptr]);
    // io_read_char(IO_STDIN);
}

/* Since brainfuck does not have any special
 * keywords, it is ok to skip lexical analysis.
 *
 * TODO:
 * memory debugging
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
        bfip_execute(&memb, args.script, args.debug ? bfip_debug_callback : NULL);
        memb_cleanup(&memb);
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
    bfip_execute(&memb, file_content_buf.content, args.debug ? bfip_debug_callback : NULL);

    memb_cleanup(&memb);
    buf_cleanup(&file_content_buf);
    return 0;
}

int bfip_jump_distance_from_rightbr(int ip, char *bf)
{
    if (bf[ip] != ']') {
        /* TODO:
         * actual error message
         */
        fprintf(stderr, "ERROR: logic err");
        exit(EXIT_FAILURE);
    }

    int distance = 0;
    int skip = 0;

    for (int tmp = ip - 1; tmp >= 0 && skip != -1; tmp--) {
        distance--;
        skip += (bf[tmp] == ']');
        skip -= (bf[tmp] == '[');
    }

    if (skip != -1) {
        fprintf(stderr, "ERROR: expected [, not found\n");
        exit(EXIT_FAILURE);
    }

    return distance;
}

int bfip_jump_distance_from_leftbr(int ip, char *bf)
{
    if (bf[ip] != '[') {
        /* TODO:
         * actual error message
         */
        fprintf(stderr, "ERROR: logic err");
        exit(EXIT_FAILURE);
    }

    int distance = 0;
    int skip = 0;

    for (int tmp = ip + 1; bf[ip] != '\0' && skip != -1; tmp++) {
        distance++;
        skip += (bf[tmp] == '[');
        skip -= (bf[tmp] == ']');
    }

    if (skip != -1) {
        fprintf(stderr, "ERROR: expected ], not found\n");
        exit(EXIT_FAILURE);
    }

    return distance;
}

void bfip_execute(memb_t *memb, char *bf, void (*callback)(memb_t *memb, char *bf, int ip))
{
    for (int ip = 0; bf[ip] != '\0'; ip++) {
        switch (bf[ip]) {
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
            if (memb->block[memb->ptr] == 0) {
                ip += bfip_jump_distance_from_leftbr(ip, bf);
            }

            break;
        }
        case ']': {
            if (memb->block[memb->ptr] != 0) {
                ip += bfip_jump_distance_from_rightbr(ip, bf);
            }

            break;
        }
        default:
            break;
        }

        if (callback != NULL) {
            callback(memb, bf, ip);
        }
    }
}
