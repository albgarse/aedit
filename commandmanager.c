#include <string.h>
#include <stdlib.h>
#include "datatypes.h"
#include "commandmanager.h"
#include "datamanager.h"
#include "iomanager.h"


int processCommand(char cmd[_STR_SIZE], struct textBuffer *b)
{
  char tokens[_MAX_TOKENS][_STR_SIZE];
  char filename[_STR_SIZE];
  int ntokens;
  int result;

  ntokens = tokenize(cmd, tokens);

  if (ntokens > 0) {
    /* quit command */
    if (tokens[0][0]=='q') {
      /* return 1 to force main to exit */
      return 1;
    } else

    /* save as command */
    if (tokens[0][0]=='w' && tokens[0][1]=='a') {
      readCommand("Enter file name: ", filename);
      strncpy(b->bufferName, filename, _STR_SIZE);
      if (b->bufferName[0] != '\0') {
        result = save(b, b->bufferName);
        if (result == _KO) {
          strncpy(b->lastError, "Error Saving File", _STR_SIZE);
        }
      } else {
        strncpy(b->lastError, "The file was not saved", _STR_SIZE);
      }
    } else

    /* save command */
    if (tokens[0][0]=='w') {
      if (b->bufferName[0] == '\0') {
        readCommand("Enter file name: ", filename);
        strncpy(b->bufferName, filename, _STR_SIZE);
      }
      if (b->bufferName[0] != '\0') {
        result = save(b, b->bufferName);
        if (result == _KO) {
          strncpy(b->lastError, "Error Saving File", _STR_SIZE);
        }
      } else {
        strncpy(b->lastError, "The file was not saved", _STR_SIZE);
      }
    } else

    /* mark to select text */
    if (tokens[0][0]=='s') {
      if (b->mark_init == b->mark_end) {
        /* first mark */
        b->mark_init = b->leftLength + 1;
      } else {
        /* there is a mark yet. Update */
        if (b->mark_init < b->leftLength) {
          b->mark_end = b->leftLength;
        } else {
          b->mark_end = b->mark_init;
          b->mark_init = b->leftLength;
        }
      }
    } else

    /* deselected text */
    if (tokens[0][0]=='S') {
      b->mark_init = b->mark_end = 0;
    } else

    /* copy text */
    // if (tokens[0][0]=='c') {
    //   copy(b);
    // }

    /* past text */
    if (tokens[0][0]=='c') {
      copy(b);
      paste(b);
    } else

    /* delete selected text */
    if (tokens[0][0]=='d') {
      delselected(b);
    } else

    /* move selected text */
    if (tokens[0][0]=='m') {
      copy(b);
      paste(b);
      delselected(b);
    }
  }
  return 0;
}



int tokenize(char cmd[_STR_SIZE], char tokens[_MAX_TOKENS][_STR_SIZE])
{
  int i, j, token;

  j=0;
  token=0;
  for (i=0; cmd[i]!='\0' && i<_STR_SIZE; i++) {
    if (cmd[i] != ' ') {
      tokens[token][j++] = cmd[i];
    } else {
      tokens[token][j]='\0';
      if (token<_MAX_TOKENS) {
        token++;
        j=0;
        /* eliminate extra spaces */
        while (cmd[i+1] == ' ') {
          i++;
        }
      }
    }
  }

  return token+1;
}
