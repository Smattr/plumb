.PHONY: default
default: plumb

CC = gcc

ifeq (${V},1)
Q := 
else
Q := @
endif

# Lazy source calculation. Assume every C file needs to be compiled and linked
# and every .o file depends on every header.
SOURCES = $(sort $(wildcard *.c))
$(SOURCES:%.c=%.o): $(filter-out main.h,$(SOURCES:%.c=%.h))

plumb: $(SOURCES:%.c=%.o)
	@echo " [LD] $@"
	${Q}${CC} ${LDFLAGS} -o $@ $^

%.o: %.c
	@echo " [CC] $@"
	${Q}${CC} -std=c99 -c -o $@ $<

.PHONY: clean
clean:
	${Q}rm -rf plumb *.o
