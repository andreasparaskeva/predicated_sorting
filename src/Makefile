CC=gcc
# -Wformat-signedness
CFLAGS=-Wformat=2 -Wextra -Wall -Wpedantic -Werror -pedantic-errors -O3
BINARIES=\
	bubblesort-branched-inplace \
	bubblesort-predicated-inplace \
	mergesort-branched-inplace \
	mergesort-predicated-inplace

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: all clean

all: $(BINARIES)

clean:
	rm -v $(BINARIES)
