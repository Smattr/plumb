#include <stdlib.h>
#include <string.h>

#include "junction.h"
#include "process.h"
#include "util.h"

process_t *procs = NULL;

size_t in_count(int junction_id) {
    size_t count = 0;
    for (process_t *p = procs; p != NULL; p = p->next) {
        if (p->out == junction_id)
            ++count;
        if (p->err == junction_id)
            ++count;
    }
    return count;
}

size_t out_count(int junction_id) {
    size_t count = 0;
    for (process_t *p = procs; p != NULL; p = p->next) {
        if (p->in == junction_id)
            ++count;
    }
    return count;
}

int plumb_in = NULL_JUNCTION,
    plumb_out = NULL_JUNCTION,
    plumb_err = NULL_JUNCTION;

void process_add(process_t *p) {
    p->next = procs;
    procs = p;
}

void process_reset(void) {
    for (process_t *p1 = procs, *p2; p1 != NULL; p1 = p2) {
        p2 = p1->next;
        free(p1);
    }
    procs = NULL;
}
