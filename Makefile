# Makefile for aedit

LDFLAGS := -lcurses
CFLAGS := -g -oae

all:
	$(CC) $(CFLAGS) main.c datamanager.c iomanager.c screenmanager.c commandmanager.c $(LDFLAGS)

clean:
	rm -f *.o
