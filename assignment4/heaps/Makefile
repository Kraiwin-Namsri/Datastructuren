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
`pkg-config --cflags check` \
-Wstrict-prototypes
endef

# Turn on the address sanitizer
LDFLAGS = -fsanitize=address

TEST_LDFLAGS = $(LDFLAGS) `pkg-config --libs check`

PROG = queue
CHECK_HEAP = check_heap

all: $(PROG) $(CHECK_HEAP)

valgrind: LDFLAGS=-lm
valgrind: CFLAGS=-Wall -g3
valgrind: $(PROG)

heap.o: heap.c prioq.h array.h

queue: heap.o main.o array.o
	$(CC) -o $@  $^ $(LDFLAGS)

check_heap: check_heap.o heap.o array.o
	$(CC) -o $@ $^ $(TEST_LDFLAGS)

clean:
	rm -f *.o $(PROG) $(CHECK_HEAP)

tarball: prioq_submit.tar.gz

prioq_submit.tar.gz: main.c heap.c array.h array.c prioq.h Makefile
	tar -czf $@ $^

check: all
	@echo "Checking heap"
	./$(CHECK_HEAP)
	@echo "\nChecking queue:"
	./check_queue.sh
