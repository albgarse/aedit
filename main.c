#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include "datatypes.h"

void mainloop(struct textBuffer *);

int main(int argc, char **argv) {

  struct textBuffer *b;

 	if (argc<=1) {
		printf("use aedit [filename]\n");
		exit(1);
	}

	initScreen();

	b=(struct textBuffer *)malloc(sizeof(struct textBuffer));

	strncpy(b->bufferName,argv[1],255);
  load(b,argv[1]);
  moveGap(b,-1);
  display(b);

  mainloop(b);

 	closeScreen();
	freeBuffer(b);

	return 0;
}

void mainloop(struct textBuffer *b) {
  int quit,ch,i;

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
      //b->curx=0;
      //b->cury++;
      break;

    case 27:
      quit=1;
      break;

    case KEY_BACKSPACE:
      delCurrentChar(b);
      break;

    case 330:
      moveGap(b,b->leftLength+1);
      delCurrentChar(b);
      break;

    default:
      insert(b,(char)ch);
      break;
    }

    display(b);
  }
}
