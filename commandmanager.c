#include <string.h>
#include "datatypes.h"

int processCommand(char cmd[_STR_SIZE], struct textBuffer *b)
{
  char tokens[_MAX_TOKENS][_STR_SIZE];
  int ntokens;
  int result;

  ntokens = tokenize(cmd, tokens);

  if (ntokens > 0) {
    /* quit command */
    if (tokens[0][0]=='q') {
      return 1;
    }

    /* save command */
    if (tokens[0][0]=='s') {
      result = save(b, b->bufferName);
      if (result == _KO) {
        strncpy(b->lastError, "Error Saving File", _STR_SIZE);
      }
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
