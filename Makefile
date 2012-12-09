.PHONY: default
default: plumb

CC = gcc

ifeq (${V},1)
Q := 
else
Q := @
endif

# Lazy source calculation. Assume every C file needs to be compiled and linked
# and every C file depends on every header.
SOURCES = $(sort $(wildcard *.c))
${SOURCES}: $(filter-out main.h,$(SOURCES:%.c=%.h))

plumb: $(SOURCES:%.c=%.o)
	${Q}${CC} ${LDFLAGS} -o $@ $^

%.o: %.c
	${Q}${CC} -std=c99 -c -o $@ $^

.PHONY: clean
clean:
	${Q}rm -rf plumb *.o
