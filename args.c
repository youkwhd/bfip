#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

#include "args.h"

void args_print_usage_and_exit(char *progname, int exit_status)
{
    fprintf(stdout,
            "Usage: %s [options ...] [-e script | file]\n"
            "Brainfuck interpreter.\n"
            "\n"
            "Options:\n"
            "   -e script, --eval script    evaluates the given script.\n"
            "   -h, --help                  prints this message and exit.\n",
            progname);
    exit(exit_status);
}

void args_parse(args_t *args, int argc, char **argv)
{
    char *short_opts = ":e:h:";
    struct option long_opts[] = {
        { "eval", no_argument, NULL, 'e' },
        { "help", no_argument, NULL, 'h' },
        { 0 }
    };

    int opt, long_opts_index;

    while ((opt = getopt_long(argc, argv, short_opts, long_opts, &long_opts_index)) != -1) {
        switch (opt) {
        case 'e':
            args->script = optarg;
            args->use_repl = false;
            break;
        /* When valid but missing something
         */
        case ':':
            fprintf(stderr, 
                    "%s: Insufficient argument '%c'\n"
                    "Try `%s --help` for more information.\n",
                    argv[0], optopt, argv[0]);
            exit(EXIT_FAILURE);
            break;
        /* Unknown flag
         */
        case '?':
            /* optopt:
             * short opts char if error
             *
             * this is the double dash version
             * the other is single dash error
             */
            if (optopt == 0) {
                fprintf(stderr, 
                        "%s: Invalid option '%s'\n"
                        "Try `%s --help` for more information.\n",
                        argv[0], argv[optind - 1], argv[0]);
            } else {
                fprintf(stderr, 
                        "%s: Invalid option '%c'\n"
                        "Try `%s --help` for more information.\n",
                        argv[0], optopt, argv[0]);
            }
            exit(EXIT_FAILURE);
        case 'h':
        default:
            args_print_usage_and_exit(argv[0], EXIT_SUCCESS);
            break;
        }
    }

    /* see: https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
     */
    if (optind < argc) {
        args->use_repl = false;
        args->file = argv[optind];
    }

    /* TODO:
     * implement REPL
     */
    if (args->use_repl) {
        args_print_usage_and_exit(argv[0], EXIT_FAILURE);
    }

    if (args->file == NULL && !args->use_repl && args->script == NULL) {
        args_print_usage_and_exit(argv[0], EXIT_FAILURE);
    }
}

void args_init(args_t *args, int argc, char **argv)
{
    args->file = NULL;
    args->script = NULL;
    args->use_repl = true;
    args_parse(args, argc, argv);
}
