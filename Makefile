# Makefile for aedit

LDFLAGS := -lcurses
CFLAGS := -g -oaedit

all:
	$(CC) $(CFLAGS) main.c datamanager.c iomanager.c screenmanager.c  $(LDFLAGS)

clean:
	rm -f *.o