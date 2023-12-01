CC=gcc
CFLAGS=-std=c11 -Iinclude
SRCDIR=src
INCDIR=include
OBJDIR=obj
BINDIR=bin

all: $(BINDIR)/run.out

$(BINDIR)/run.out: $(OBJDIR)/main.o $(OBJDIR)/operations.o
	$(CC) $^ -o $@ -lpthread

$(OBJDIR)/main.o: $(SRCDIR)/main.c $(INCDIR)/globals.h $(INCDIR)/operations.h
	$(CC) -c $< $(CFLAGS) -o $@

$(OBJDIR)/operations.o: $(SRCDIR)/operations.c $(INCDIR)/operations.h $(INCDIR)/globals.h
	$(CC) -c $< $(CFLAGS) -o $@

.PHONY: clean

clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/run.out
