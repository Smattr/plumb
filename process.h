#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <sys/types.h>

typedef struct _process {
    /* Junction index to connect each FD to or NULL_JUNCTION to effectively
     * specify /dev/null.
     */
    int in;
    int out;
    int err;

    /* FDs after the process has been started. */
    int in_fd;
    int out_fd;
    int err_fd;

    /* Command to execute. */
    char *command;

    /* PID after the process has been started. */
    pid_t pid;

    /* Linked-list pointer. */
    struct _process *next;
} process_t;
extern process_t *procs;

/* Junction indices for our own FDs. */
extern int plumb_in, plumb_out, plumb_err;

void process_add(process_t *p);
void process_reset(void);

size_t in_count(int junction_id);
size_t out_count(int junction_id);

#endif
