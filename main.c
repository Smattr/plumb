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

    for (proc_entry_t *p = procs; p != NULL; p = p->next) {
        if (start(p->proc->command,
            &p->proc->in_fd,
            &p->proc->out_fd,
            &p->proc->err_fd,
            &p->proc->pid)) {
            fprintf(stderr, "Failed to start '%s'\n", p->proc->command);
            return -1;
        }
    }

    for (proc_entry_t *p = procs; p != NULL; p = p->next) {
        junction_t *j;
        assert(p->proc != NULL);
        if (p->proc->in != NULL_JUNCTION) {
            assert(p->proc->in <= max_junction);
            j = &junctions[p->proc->in];
            j->out[j->out_sz++] = p->proc->in_fd;
        }
        if (p->proc->out != NULL_JUNCTION) {
            assert(p->proc->out <= max_junction);
            j = &junctions[p->proc->out];
            j->in[j->in_sz++] = p->proc->out_fd;
        }
        if (p->proc->err != NULL_JUNCTION) {
            assert(p->proc->err <= max_junction);
            j = &junctions[p->proc->err];
            j->in[j->in_sz++] = p->proc->err_fd;
        }
    }

    /* TODO: spawn party */


    return 0;
}

