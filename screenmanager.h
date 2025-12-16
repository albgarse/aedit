/****
 screenmanager.h
 (c)2003 Alberto Garcia Serrano.

 screen management routines
****/


/****
The load function loads a file from disk into the text buffer.
the file parametes is the string containing the full filename (including path).
****/

#include "datamanager.h"

void display(struct textBuffer *buffer);

void initScreen();
void closeScreen();
void readCommand(char *, char *);

void showHelpWindow();
