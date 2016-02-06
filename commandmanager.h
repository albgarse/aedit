/****
 commandmanager.h
 (c)Alberto Garcia Serrano
 This software is under GPL license.
 ****/

 int tokenize(char cmd[_STR_SIZE], char tokens[_MAX_TOKENS][_STR_SIZE]);
 int processCommand(char cmd[_STR_SIZE], struct textBuffer *b);
