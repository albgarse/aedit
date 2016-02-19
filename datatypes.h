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
#define _STR_SIZE 255
#define _MAX_TOKENS 4
#define _MIN_GAP_SIZE 10
#define TRUE 1
#define FALSE 0

/* KEYS */
#define KEY_SUP 337
#define KEY_SDOWN 336
/* KEY_SLEFT & KEY_SRIGHT are yet defined in ncurses */
#define KEY_SUPR 330
#define KEY_ESC 27

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
	char bufferName[_STR_SIZE];	/* name of the buffer 				*/
	short int curx,cury;	/* actual cursor position on screen TODO: delete		*/
	int mark_init, mark_end; /* pointers to marked text */
	int texty;		/* actual cursor position on text 		*/
	int modified; /* the has been modified since last save */
	char lastError[_STR_SIZE]; /* last error */
	unsigned char *copybuffer; /* buffer for copy&paste operations */
	int copybufferLength; /* length of the data in the copybuffer */
};
