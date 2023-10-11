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

void bfip_debug_callback_before(memb_t *memb, char *program, int ip)
{
    printf("[DEBUG ::] INST (%d := %c) MEMORY (%d := %d)\n", ip, program[ip], memb->ptr, memb->block[memb->ptr]);
}

void bfip_debug_callback_after(memb_t *memb, char *program, int ip)
{
    UNUSED(memb);

    if (program[ip] == '.') {
        putchar('\n');
    }
}

void bfip_debug_callback_with_step_before(memb_t *memb, char *program, int ip)
{
    printf("[DEBUG ::] INST (%d := %c) MEMORY (%d:%d)", ip, program[ip], memb->ptr, memb->block[memb->ptr]);
    getchar();
}

#define bfip_debug_callback_with_step_after bfip_debug_callback_after

/* Since brainfuck does not have any special
 * keywords, it is ok to skip lexical analysis.
 */
int main(int argc, char **argv)
{
    args_t args;
    args_init(&args, argc, argv);

    memb_t memb;
    buf_t file_content_buf;

    if (!memb_init(&memb, MEMB_INITIAL_SIZE)) {
        exit(5);
    }

    void *callback_fn_before = NULL;
    void *callback_fn_after = NULL;

    if (args.debug && args.step) {
        callback_fn_before = bfip_debug_callback_with_step_before;
        callback_fn_after = bfip_debug_callback_with_step_after;
    } else if (args.debug) {
        callback_fn_before = bfip_debug_callback_before;
        callback_fn_after = bfip_debug_callback_after;
    } 

    /* flag -e specified
     */
    if (args.script != NULL) {
        bfip_execute(&memb, args.script, callback_fn_before, callback_fn_after);
        memb_cleanup(&memb);
        exit(EXIT_SUCCESS);
    }

    if (!buf_init(&file_content_buf, BUF_INITIAL_SIZE)) {
        memb_cleanup(&memb);
        exit(2);
    }

    /* TODO: 
     * Calling io_open() like this sucks
     */
    io_fd_t fd = io_open(args.file, O_RDONLY);

    if (!io_read(&file_content_buf, fd)) {
        memb_cleanup(&memb);
        buf_cleanup(&file_content_buf);
        io_close(fd);
        exit(3);
    }

    io_close(fd);

    /* TODO: 
     * Execute Brainfuck without buffering
     */
    bfip_execute(&memb, file_content_buf.content, callback_fn_before, callback_fn_after);

    memb_cleanup(&memb);
    buf_cleanup(&file_content_buf);
    return EXIT_SUCCESS;
}

int bfip_jump_distance_from_rightbr(int ip, char *program)
{
    if (program[ip] != ']') {
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
        skip += (program[tmp] == ']');
        skip -= (program[tmp] == '[');
    }

    if (skip != -1) {
        fprintf(stderr, "ERROR: expected [, not found\n");
        exit(EXIT_FAILURE);
    }

    return distance;
}

int bfip_jump_distance_from_leftbr(int ip, char *program)
{
    if (program[ip] != '[') {
        /* TODO:
         * actual error message
         */
        fprintf(stderr, "ERROR: logic err");
        exit(EXIT_FAILURE);
    }

    int distance = 0;
    int skip = 0;

    for (int tmp = ip + 1; program[tmp] != '\0' && skip != -1; tmp++) {
        distance++;
        skip += (program[tmp] == '[');
        skip -= (program[tmp] == ']');
    }

    if (skip != -1) {
        fprintf(stderr, "ERROR: expected ], not found\n");
        exit(EXIT_FAILURE);
    }

    return distance;
}

void bfip_execute(memb_t *memb, char *program, void (*callback_fn_before)(memb_t *memb, char *program, int ip), void (*callback_fn_after)(memb_t *memb, char *program, int ip))
{
    for (int ip = 0; program[ip] != '\0'; ip++) {
        if (callback_fn_before != NULL && program[ip] != ' ' && program[ip] != '\n') {
            callback_fn_before(memb, program, ip);
        }

        switch (program[ip]) {
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
            int distance = bfip_jump_distance_from_leftbr(ip, program);

            if (memb->block[memb->ptr] == 0) {
                ip += distance;
            }

            break;
        }
        case ']': {
            int distance = bfip_jump_distance_from_rightbr(ip, program);

            if (memb->block[memb->ptr] != 0) {
                ip += distance;
            }

            break;
        }
        default:
            break;
        }

        if (callback_fn_after != NULL && program[ip] != ' ' && program[ip] != '\n') {
            callback_fn_after(memb, program, ip);
        }
    }
}
