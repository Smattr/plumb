#ifndef _UTIL_H_
#define _UTIL_H_

#include <string.h>
#include <sys/types.h>

void *xalloc(size_t size);
int to_int(char *s);
int start(char *command, int *in, int *out, int *err, pid_t *pid);

#endif
