#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include "datatypes.h"
#include "datamanager.h"
#include "iomanager.h"
#include "screenmanager.h"
#include "commandmanager.h"


void mainloop(struct textBuffer *);

int main(int argc, char **argv) {

  struct textBuffer *b;

  b=(struct textBuffer *)malloc(sizeof(struct textBuffer));

 	if (argc<=1) {
    b->bufferName[0] = '\0';
    if (createBuffer(b, _INIT_SIZE) != _OK) {
      printf("Error creating initial buffer.\n");
      return _KO;
    }
		moveGap(b,-1);
	} else {
    strncpy(b->bufferName,argv[1],_STR_SIZE);
    load(b,argv[1]);
    moveGap(b,-1);
  }

	initScreen();
  display(b);

  mainloop(b);

 	closeScreen();
	freeBuffer(b);

	return 0;
}

void mainloop(struct textBuffer *b) {
  int quit,i;
  int ch;
  char cmd[_STR_SIZE];

  quit=0;

  while (!quit) {
    ch=getch();

    switch (ch) {

    case KEY_UP:
      if (b->cury <= 1)
      // Scroll down
      movePreviousLine(b);
      moveCursorUp(b);
      break;


    case KEY_DOWN:
      if (b->cury >= LINES-1)
      // Scroll up
      moveNextLine(b);
      moveCursorDown(b);
    break;

    case KEY_LEFT:
      cursorLeft(b);
      break;

    case KEY_RIGHT:
      cursorRight(b);
      break;

    case KEY_NPAGE:
      for (i=1 ; i<=_LINESINPAGE ; i++) {
        moveNextLine(b);
        moveCursorDown(b);
      }
      break;

    case KEY_PPAGE:
      for (i=1 ; i<=_LINESINPAGE ; i++) {
        movePreviousLine(b);
        moveCursorUp(b);
      }
      break;

    case 10:
    case 13:
      insert(b,'\n');
      break;

    case 27:
      readCommand("cmd> ", cmd);
      quit = processCommand(cmd, b);
      break;

    case KEY_BACKSPACE:
    case 8:
    case 127:
      delCurrentChar(b);
      break;

    case 330:
      moveGap(b,b->leftLength+1);
      delCurrentChar(b);
      break;

    case 23: /* ^W */
      save(b, b->bufferName);
      break;

    default:
      insert(b,(char)ch);
      break;
    }

    display(b);
  }
}
