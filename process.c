#include <stdlib.h>

#include "process.h"
#include "util.h"

struct proc_entry {
    struct process *proc;
    struct proc_entry *next;
};
static struct proc_entry *procs = NULL;

int plumb_in = -1,
    plumb_out = -1,
    plumb_err = -1;

void process_add(struct process *p) {
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
