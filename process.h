#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <sys/types.h>

typedef struct {
    int in; int in_fd;
    int out; int out_fd;
    int err; int err_fd;
    char *command;
    pid_t pid;
} process_t;

extern int plumb_in, plumb_out, plumb_err;

void process_add(process_t *p);
void process_reset(void);

typedef struct proc_entry {
    process_t *proc;
    struct proc_entry *next;
} proc_entry_t;
extern proc_entry_t *procs;

size_t in_count(int junction_id);
size_t out_count(int junction_id);


#endif
