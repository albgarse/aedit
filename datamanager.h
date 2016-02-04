/**** 
 datamanager.h
 (c)Alberto Garcia Serrano
 This software is under GPL license.

 Main data structures definitions for the editor.
 This editor is based on the gap buffer implementation as described in
 Emacs cookbook. This is the aproach used un Emacs as well.
****/


/****
 The createBuffer funtion creates a new buffer
 buffer is a pointer to the new buffer
 size is the initial size for the buffer
****/
int createBuffer(struct textBuffer *buffer, int size);

void freeBuffer(struct textBuffer *buffer);
/****
 The moveGap function takes care of gap movements into the text buffer.
 buffer is the buffer who we want to move the gap.
 position is the new location in the text buffer.
****/
void moveGap(struct textBuffer *buffer, int position);


/****
 The newSize funtion realocates the the text buffer memory as needed.
 newzize parameter is the new size needed in the buffer.
****/
int newSize(struct textBuffer *buffer, int newsize);


/****
 checkSize is called to check if there is enought room for insertion,
 if not, this function realocates new espace calling he newSize function.
 insLen is the length of the text we want to insert.
****/
//void checkSize(struct textBuffer *buffer, int insLen);


/****
 The charAt function returns the char at a given position.
****/
//char charAt(struct textBuffer *buffer, int position);


/****
 The insertText function inserts a string into one buffer.
 Position is the location to insert the text.
 newText is the text we want to insert.
 textLength if the length of the newText parameter.
****/
//void insertText(struct textBuffer *buffer, int position, char *newText, int textLength);

void movePreviousLine(struct textBuffer *buffer);

void moveNextLine(struct textBuffer *buffer);

void moveCursorUp(struct textBuffer *buffer);

void moveCursorDown(struct textBuffer *buffer);

int inGap(struct textBuffer *buffer, unsigned char *p);

void insert(struct textBuffer *buffer, unsigned char ch);

void delCurrentChar(struct textBuffer *buffer);

void cursorLeft(struct textBuffer *buffer);

void cursorRight(struct textBuffer *buffer);
