#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arguments.h"
#include "junction.h"
#include "process.h"
#include "util.h"

int main(int argc, char **argv) {
    parse_options(argc, argv);

    junctions = (junction_t*)malloc(sizeof(junction_t) * max_junction);
    if (junctions == NULL) {
        fprintf(stderr, "Out of memory when creating junctions\n");
        return -1;
    }

    for (int i = 0; i < max_junction; ++i) {
        junctions[i].in = (int*)malloc(sizeof(int) * in_count(i));
        junctions[i].out = (int*)malloc(sizeof(int) * out_count(i));
        if (!junctions[i].in || !junctions[i].out) {
            fprintf(stderr, "Out of memory while allocating junction %d\n", i);
            return -1;
        }
        junctions[i].in_sz = junctions[i].out_sz = 0;
    }

    for (process_t *p = procs; p != NULL; p = p->next) {
        if (start(p->command,
            &p->in_fd,
            &p->out_fd,
            &p->err_fd,
            &p->pid)) {
            fprintf(stderr, "Failed to start '%s'\n", p->command);
            return -1;
        }
    }

    for (process_t *p = procs; p != NULL; p = p->next) {
        junction_t *j;
        if (p->in != NULL_JUNCTION) {
            assert(p->in <= max_junction);
            j = &junctions[p->in];
            j->out[j->out_sz++] = p->in_fd;
        }
        if (p->out != NULL_JUNCTION) {
            assert(p->out <= max_junction);
            j = &junctions[p->out];
            j->in[j->in_sz++] = p->out_fd;
        }
        if (p->err != NULL_JUNCTION) {
            assert(p->err <= max_junction);
            j = &junctions[p->err];
            j->in[j->in_sz++] = p->err_fd;
        }
    }

    /* TODO: spawn party */


    return 0;
}

