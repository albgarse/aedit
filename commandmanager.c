#include <string.h>
#include <stdlib.h>
#include "datatypes.h"
#include "commandmanager.h"
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
      if (b->copybufferLength != 0) {
        /* free copy buffer */
        free(b->copybuffer);
      }
      free(b->data);

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
        b->mark_init = b->leftLength+1;
      } else {
        /* there is a mark yet. Update */
        b->mark_end = b->leftLength+1;
      }
    }

    /* deselected text */
    if (tokens[0][0]=='M') {
      b->mark_init = b->mark_end = 0;
    }

    /* copy text */
    if (tokens[0][0]=='c') {
      copy(b);
    }

    /* past text */
    if (tokens[0][0]=='p') {
      paste(b);
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
