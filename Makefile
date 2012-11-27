.PHONY: default
default: plumb

CC = gcc

ifeq (${V},1)
Q := 
else
Q := @
endif

SOURCES = \
    arguments.c \
    main.c \
    process.c \
    util.c \

plumb: $(SOURCES:%.c=%.o)
	${Q}${CC} ${LDFLAGS} -o $@ $^

%.o: %.c
	${Q}${CC} -std=c99 -c -o $@ $^

.PHONY: clean
clean:
	${Q}rm -rf plumb *.o
