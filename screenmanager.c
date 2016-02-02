#include <stdio.h>
#include <string.h>
#include <curses.h>
#include "datatypes.h"
#include "datamanager.h"
#include "screenmanager.h"


void display(struct textBuffer *buffer) {

 unsigned char *p;
 int y,x,i,j;
 int xtmp,ytmp;
 int charWrote,newline;
 char topLine[COLS+50];

 /* x and y are the position in screen to write the next char */
	x=0;
	y=1;
	p=buffer->scrtop;

	while (y < LINES) {

		charWrote=FALSE;
		newline=FALSE;

		/* mind the gap! */
		/* if we are inside the gap, we have to go to the end of it */
		if (!inGap(buffer,p)) {
			p = buffer->data+buffer->leftLength+buffer->gapLength+1;
      xtmp = x;
      ytmp = y;
    }

		switch (*p) {

		case '\t':
			i=1;
			while (i++%_TABLENGTH) mvaddch(y,x++,' ');
			break;

		case '\n':
			/* clear rest of the line */
			for (i=x ; i<=COLS ; i++) mvaddch(y,x++,' ');
		//	xtmp=buffer->curx+1;
		//	ytmp=buffer->cury;
			newline = TRUE;
			y++;
			x=0;
			break;

		default:
			/* match end of line? */
			if (x>=COLS) {
				x=0;
				y++;
			}


			/* match end of file? */
			if (p >= buffer->data+buffer->gapLength+buffer->length) {

				/* clear the rest of the windows */
				for (j=y ; j<=LINES ; j++) {
					for (i=x ; i<=COLS ; i++) mvaddch(y,x++,' ');
					y++;
					x=0;
				}
			}


			/* write em! */
			mvaddch(y,x,(unsigned char)*p);

			x++;
			charWrote=TRUE;

			break;
		}

		/* check if this is the cursor position */
		if (newline != TRUE) {
			if ((p-buffer->data) ==  buffer->leftLength+buffer->gapLength+1) {
			//	buffer->curx=charWrote==TRUE?x-1:x;
			//	buffer->cury=y;
			}
		} else {
			/* newline char matched! */
			/* set the cursor after the last char of the line */
			if ((p-buffer->data) ==  buffer->leftLength+buffer->gapLength+1) {
			//	buffer->curx=xtmp;
			//	buffer->cury=ytmp;
			}
		}

		p++;
	}

	/* Draw the top line */
	//attron(COLOR_PAIR(_COLOR_INV));
  attron(A_REVERSE);
	sprintf(topLine," aedit v0.1b   File: %s  Row %d  Col %d ",buffer->bufferName,buffer->texty+1,buffer->curx+1);
//sprintf(topLine,"leftLength: %d  gapLength: %d Length: %d",buffer->leftLength,buffer->gapLength,buffer->length);

  for (i=1 ; i<=COLS ; i++)
		strcat(topLine," ");
	mvaddnstr(0,0,topLine,COLS);
	//attron(COLOR_PAIR(_COLOR_NORM));

	/* set cursor position */
  	buffer->curx=xtmp;
  	buffer->cury=ytmp;
	move(buffer->cury,buffer->curx);
  attroff(A_REVERSE);
}




void initScreen() {

        initscr();

        noecho();
        cbreak();
	keypad(stdscr,TRUE);
	nonl();


	if (has_colors()) {
		//start_color();


		//init_pair(_COLOR_NORM, COLOR_GREEN, COLOR_BLACK);
		//init_pair(_COLOR_INV, COLOR_BLUE, COLOR_GREEN);

/*
		init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);
		init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
		init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
		init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
		init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
		init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
		init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
		init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
*/
		//assume_default_colors(COLOR_GREEN,COLOR_BLUE);

		//attron(COLOR_PAIR(_COLOR_NORM));
	}
}


void closeScreen() {
	endwin();
}
