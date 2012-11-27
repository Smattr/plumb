#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <sys/types.h>

struct process {
    int in;
    int out;
    int err;
    char *command;
    pid_t pid;
};

extern int plumb_in, plumb_out, plumb_err;

void process_add(struct process *p);

#endif
