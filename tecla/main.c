#include <stdio.h>
#include <curses.h>

int main(int argc, char **argv) {

 int ch;

	initscr();
	keypad(stdscr,TRUE);
	ch=getch();
	endwin();
	
	printf("\n%d\n",ch);
	
	return 1;
}
