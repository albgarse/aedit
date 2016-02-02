/****
 datatypes.h
 (c)Alberto Garcia Serrano
 This software is under GPL license.

 Main data structures definitions for the editor.
 This editor is based on the gap buffer implementation as described in
 Emacs cookbook. This is the aproach used in Emacs as well.
****/

#define _OK 0
#define _KO 1
#define _LINESINPAGE LINES/2
#define _TABLENGTH 8
#define _COLOR_NORM 1
#define _COLOR_INV 2
#define _INIT_SIZE 1024



/****
 The textBuffer structure fits one text buffer
 the data field contains the text itself plus the gap buffer.
 the size field represents the total amount of memory used to fit the text plus the gap buffer.
 the lengh field represents the number of characters in text (size - gapLength).
 the leftLength contains the length of the first part of the text buffer.
 the gapLength field contains the gap size in bytes.
 the growSize is for buffer realocation porpouses.
 ****/
struct textBuffer {
	unsigned char *data;	/* text data 					*/
	int size;		/* buffer size 					*/
	int length;		/* total length (in chars) 			*/
	int leftLength;		/* total length of the left buffer 		*/
	int gapLength;		/* length of the gap 				*/
	int growSize;		/* step size when the buffer grows 		*/
	unsigned char *scrtop;	/* pointer to the text at the top of the screen */
	char bufferName[255];	/* name of the buffer 				*/
	short int curx,cury;	/* actual cursor position on screen 		*/
	int texty;		/* actual cursor position on text 		*/
};
