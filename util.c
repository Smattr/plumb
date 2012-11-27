#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
