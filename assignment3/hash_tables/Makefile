# Change this to change the compiler used
# CC = clang
CC = gcc

# Turn on the address sanitizer
LDFLAGS = -fsanitize=address -fno-omit-frame-pointer -ldl -lm

# To turn off the address sanitizer, instead use
# LDFLAGS = -fno-omit-frame-pointer -ldl -lm

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

# Flags needed for the check library
CHECK_LDFLAGS = $(LDFLAGS) `pkg-config --libs check`

PROG = lookup
TESTS = check_array check_hash_simple check_hash_array check_hash_resize check_hash_delete

all: $(PROG) $(TESTS)

valgrind: LDFLAGS=-lm
valgrind: CFLAGS=-Wall
valgrind: $(PROG)

lookup: array.o hash_table.o hash_func.o main.o
	$(CC) -o $@  $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o $(PROG) $(TESTS)

tarball: hash_table_submit.tar.gz

hash_table_submit.tar.gz: main.c array.c hash_table.c hash_func.c hash_func.h
	tar -czf $@ $^

check_array: check_array.o array.o
	$(CC) -o $@ $^ $(CHECK_LDFLAGS)

check_hash_simple: check_hash_simple.o array.o hash_func.o hash_table.o
	$(CC) -o $@ $^ $(CHECK_LDFLAGS)

check_hash_resize: check_hash_resize.o array.o hash_func.o hash_table.o
	$(CC) -o $@ $^ $(CHECK_LDFLAGS)

check_hash_array: check_hash_array.o array.o hash_func.o hash_table.o
	$(CC) -o $@ $^ $(CHECK_LDFLAGS)

check_hash_delete: check_hash_delete.o array.o hash_func.o hash_table.o
	$(CC) -o $@ $^ $(CHECK_LDFLAGS)

check: all
	@echo "\nChecking array basics..."
	./check_array
	@echo "\nChecking hash table basics..."
	./check_hash_simple
	@echo "\nChecking hash table resize..."
	./check_hash_resize
	@echo "\nChecking hash table value array..."
	./check_hash_array
	@echo "\nChecking hash table delete..."
	./check_hash_delete
	@echo "\nChecking lookup table output..."
	./check_lookup.sh

