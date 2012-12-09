#ifndef _JUNCTION_H_
#define _JUNCTION_H_

#include <stdlib.h>

#define NULL_JUNCTION (-1)

extern int max_junction;

typedef struct {
    int *in;
    size_t in_sz;
    int *out;
    size_t out_sz;
} junction_t;

extern junction_t *junctions;

#endif
