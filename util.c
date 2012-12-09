#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "util.h"

#define SHELL "/bin/sh"

void *xalloc(size_t size) {
    void *p = malloc(size);
    if (p == NULL) {
        fprintf(stderr, "xalloc failed\n");
        exit(1);
    } else {
        memset(p, 0, size);
        return p;
    }
}

int to_int(char *s) {
    for (int i = 0; s[i] != '\0'; ++i) {
        if (s[i] < '0' || s[i] > '9') {
            return -1;
        }
    }
    return atoi(s);
}

int start(char *command, int *in, int *out, int *err, pid_t *pid) {
    int fd_in[2], fd_out[2], fd_err[2];
    if (pipe(fd_in) || pipe(fd_out) || pipe(fd_err))
        return -1;
    fflush(stdout);
    fflush(stderr);
    *pid = fork();
    switch (*pid) {
        case -1: /* Fork failed */ {
            return -1;
        } case 0: /* Child */ {
            close(fd_in[1]);
            dup2(STDIN_FILENO, fd_in[0]);
            close(fd_out[0]);
            dup2(STDOUT_FILENO, fd_out[1]);
            close(fd_err[0]);
            dup2(STDERR_FILENO, fd_err[1]);
            execl(SHELL, "sh", "-c", command, NULL);
            assert(!"Unreachable");
        } default: /* Parent */ {
            close(fd_in[0]);
            *in = fd_in[1];
            close(fd_out[1]);
            *out = fd_out[0];
            close(fd_err[1]);
            *err = fd_err[0];
            return 0;
        }
    }
}
