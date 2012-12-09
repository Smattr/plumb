#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#define __USE_XOPEN_EXTENDED
#include <string.h>

#include "arguments.h"
#include "junction.h"
#include "process.h"
#include "util.h"

static char *argv0 = NULL;
static void usage(FILE *stream) {
    assert(argv0 != NULL);
    fprintf(stream,
"Usage: %s options\n"
"\n"
    , argv0);
}

static process_t *new_process(char *command) {
    process_t *p = (process_t*)malloc(sizeof(process_t));
    if (p == NULL) {
        return NULL;
    }
    if (command != NULL) {
        p->command = strdup(command);
        if (p->command == NULL) {
            free(p);
            return NULL;
        }
    }
    p->in = NULL_JUNCTION;
    p->out = NULL_JUNCTION;
    p->err = NULL_JUNCTION;
    p->pid = 0;
    return p;
}

static void set(int *target, char *val) {
    assert(target != NULL);
    int i = to_int(val);
    if (i == -1) {
        fprintf(stderr, "Illegal junction ID '%s'\n", val);
        exit(1);
    }
    if (i > max_junction) max_junction = i;
    *target = i;
}

int parse_options(int argc, char **argv) {
    static struct option opts[] = {
        {"command", required_argument, 0, 'c'},
        {"help", no_argument, 0, '?'},
        {"stderr", required_argument, 0, '2'},
        {"stdin", required_argument, 0, '0'},
        {"stdout", required_argument, 0, '1'},
        {0, 0, 0, 0},
    };

    assert(argv0 == NULL);
    argv0 = strdup(argv[0]);
    if (argv0 == NULL) {
        return -1;
    }

    process_t *current = NULL;

    while (1) {
        int opt_ind, c = getopt_long(argc, argv, "0:1:2:c:?", opts, &opt_ind);
        if (c == -1) {
            break;
        }

        switch (c) {
            case 0: {
                if (opts[opt_ind].flag != 0) {
                    continue;
                }
                fprintf(stderr, "Unexpected option %s\n", opts[opt_ind].name);
                break;
            } case '?': {
                usage(stdout);
                exit(0);
            } case 'c': {
                if (current != NULL) {
                    process_add(current);
                }
                current = new_process(optarg);
                if (current == NULL) {
                    goto fail;
                }
                break;
            } case '0': {
                set(current ? &current->in : &plumb_in, optarg);
                break;
            } case '1': {
                set(current ? &current->out : &plumb_out, optarg);
                break;
            } case '2': {
                set(current ? &current->err : &plumb_err, optarg);
                break;
            } default: {
                fprintf(stderr, "Getopt failure\n");
                exit(1);
            }
        }
    }
    if (current != NULL) {
        process_add(current);
    }
    return 0;

fail:
    perror("Failed to parse arguments");
    exit(1);
}
