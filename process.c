#include <stdlib.h>
#include <string.h>

#include "junction.h"
#include "process.h"
#include "util.h"

proc_entry_t *procs = NULL;

size_t in_count(int junction_id) {
    size_t count = 0;
    for (proc_entry_t *p = procs; p != NULL; p = p->next) {
        if (p->proc->out == junction_id)
            ++count;
        if (p->proc->err == junction_id)
            ++count;
    }
    return count;
}

size_t out_count(int junction_id) {
    size_t count = 0;
    for (proc_entry_t *p = procs; p != NULL; p = p->next) {
        if (p->proc->in == junction_id)
            ++count;
    }
    return count;
}

int plumb_in = NULL_JUNCTION,
    plumb_out = NULL_JUNCTION,
    plumb_err = NULL_JUNCTION;

void process_add(process_t *p) {
    struct proc_entry *pe = (struct proc_entry*)xalloc(sizeof(struct proc_entry));
    pe->proc = p;
    pe->next = procs;
    procs = pe;
}

void process_reset(void) {
    for (struct proc_entry *p1 = procs, *p2; p1 != NULL; p1 = p2) {
        p2 = p1->next;
        free(p1->proc);
        free(p1);
    }
    procs = NULL;
}
