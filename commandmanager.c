#include <string.h>
#include <stdlib.h>
#include "datatypes.h"
#include "commandmanager.h"
#include "datamanager.h"
#include "iomanager.h"


int processCommand(char cmd[_STR_SIZE], struct textBuffer *b)
{
  char tokens[_MAX_TOKENS][_STR_SIZE];
  int ntokens;
  int result;

  ntokens = tokenize(cmd, tokens);

  if (ntokens > 0) {
    /* quit command */
    if (tokens[0][0]=='q') {
      /* return 1 to force main to exit */
      return 1;
    }

    /* save command */
    if (tokens[0][0]=='s') {
      result = save(b, b->bufferName);
      if (result == _KO) {
        strncpy(b->lastError, "Error Saving File", _STR_SIZE);
      }
    }

    /* mark to select text */
    if (tokens[0][0]=='m') {
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
    }

    /* deselected text */
    if (tokens[0][0]=='M') {
      b->mark_init = b->mark_end = 0;
    }

    /* copy text */
    // if (tokens[0][0]=='c') {
    //   copy(b);
    // }

    /* past text */
    if (tokens[0][0]=='p') {
      copy(b);
      paste(b);
    }

    /* delete selected text */
    if (tokens[0][0]=='d') {
      delselected(b);
    }

    /* move selected text */
    if (tokens[0][0]=='v') {
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
