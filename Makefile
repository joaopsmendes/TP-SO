CC = gcc
CFLAGS = -Wall -Wextra

SRCDIR = src
BINDIR = bin

TARGETS = $(BINDIR)/monitor $(BINDIR)/tracer

.PHONY: all clean

all: $(TARGETS)

$(BINDIR)/monitor: $(SRCDIR)/monitor.c
    $(CC) $(CFLAGS) $^ -o $@

$(BINDIR)/tracer: $(SRCDIR)/tracer.c
    $(CC) $(CFLAGS) $^ -o $@

clean:
    rm -f $(TARGETS)
rm -f obj/* tmp/* bin/{tracer,monitor}