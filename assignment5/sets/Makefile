CC = gcc
#CC = clang

# Use C11, debugging and warnings flags
define CFLAGS
-std=c11 \
-g3 \
-fsanitize=address \
-Wpedantic \
-Wall \
-Wextra \
-Wformat=2 \
-Wsign-conversion \
-Wswitch-enum \
-Wno-switch-default \
-Wunused \
-Wfloat-equal \
-Wconversion \
-Wpointer-arith \
-Wtype-limits \
-Wcast-qual \
-Wenum-compare \
-Wsizeof-pointer-memaccess \
-Wstrict-prototypes \
`pkg-config --cflags check` \
-Wno-unused-parameter
endef

# Turn on the address sanitizer
LDFLAGS = -fsanitize=address

# Flags needed for the check library
CHECK_LDFLAGS = $(LDFLAGS) `pkg-config --libs check`

PROG = sets

TESTS = check_tree

all: $(PROG) $(TESTS)

tree.o: tree.c tree.h

set.o: set.c set.h tree.h

sets: main.o set.o tree.o
	$(CC) -o $@ $^ $(LDFLAGS)

valgrind: LDFLAGS=-lm
valgrind: CFLAGS=-Wall -g3
valgrind: $(PROG)

check_tree: check_tree.o tree.o
	$(CC) -o $@ $^ $(CHECK_LDFLAGS)

%.pdf: %.dot
	dot -Tpdf $< > $@

check: all
	./check_tree
	@echo
	./check_sets.sh

tarball: sets_submit.tar.gz

sets_submit.tar.gz: main.c set.c set.h tree.c tree.h Makefile
	tar -czf $@ $^

clean:
	rm -f *.o tree.dot tree.pdf $(PROG) $(TESTS) sets_valgrind
