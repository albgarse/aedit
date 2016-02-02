/****
 iomanager.h
 (c)2003 Alberto Garcia Serrano.

 Disk IO management routines
****/


/****
The load function loads a file from disk into the text buffer.
the file parametes is the string containing the full filename (including path).
****/
int load(struct textBuffer *buffer,char *file);


/****
The save function save a text buffer to a file.
the file parametes is the string containing the full filename (including path).
****/
//int save(struct textBuffer *buffer,char *file);
