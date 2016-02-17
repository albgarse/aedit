#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "datatypes.h"
#include "datamanager.h"

int createBuffer(struct textBuffer *buffer, int size) {

  int ok;

  buffer->growSize=_INIT_SIZE;
  buffer->size=0;
  buffer->length=0;
  buffer->leftLength=0;
  buffer->gapLength=0;
  buffer->curx=0;
  buffer->cury=0;
  buffer->texty=0;
  buffer->modified = 0;
  buffer->copybufferLength = 0;

  ok = newSize(buffer,size);	/* Allocate the needed space*/
  buffer->scrtop=buffer->data;

  return ok;
}



void freeBuffer(struct textBuffer *b) {
  if (b->copybufferLength != 0) {
    /* free copy buffer */
    free(b->copybuffer);
  }
  free(b->data);
}


int newSize(struct textBuffer *buffer, int size)
{
  moveGap(buffer,buffer->length);
  if (buffer->size > 0) {
    /* not a new buffer, so an old one is growing. */
    /* the old one must be copied to the new one   */
    buffer->data = realloc(buffer->data, size);
  } else {
    buffer->data = (unsigned char *)malloc(size);
  }

  buffer->gapLength+=(size-buffer->length);
  buffer->size=size;

  return _OK;
}

void moveGap(struct textBuffer *buffer, int position)
{
long amount;
unsigned char *destination,*source;

  if (position != buffer->leftLength) {
    if (position > buffer->leftLength) {
      amount = position-buffer->leftLength;
      destination=buffer->data+buffer->leftLength;
      source=buffer->data+buffer->leftLength+buffer->gapLength;
      buffer->leftLength += amount;
      memmove(destination+1,source+1,amount);
    } else {
      amount = (buffer->leftLength - position);
      destination=buffer->data+buffer->leftLength+buffer->gapLength;
      source=buffer->data+buffer->leftLength;
      buffer->leftLength -= amount;
      memmove(destination-amount+1,source-amount+1,amount);
    }
  }
}

void movePreviousLine(struct textBuffer *buffer)
{
  if (buffer->scrtop > buffer->data) {
    while ((*(--buffer->scrtop-1) != '\n') && (buffer->scrtop > buffer->data)) {
      if (!inGap(buffer,buffer->scrtop))
      buffer->scrtop=buffer->data+buffer->leftLength;
    }
  }
}



void moveNextLine(struct textBuffer *buffer)
{
  if (buffer->scrtop <= buffer->data+buffer->length+buffer->gapLength) {
    while ((*(++buffer->scrtop-1) != '\n')) {
      if (!inGap(buffer,buffer->scrtop))
      buffer->scrtop=buffer->data+buffer->leftLength+buffer->gapLength;
    }
  }
}


void moveHome(struct textBuffer *buffer)
{
  unsigned char *p, *q;

  p=q=buffer->data+buffer->leftLength;

  /* Find previous carrige return */
  while ((*(p) != '\n' && p >= buffer->data)) p--;
  moveGap(buffer,buffer->leftLength+(p-q));
}


void moveEnd(struct textBuffer *buffer)
{
  unsigned char *p, *q;;

  p=q=buffer->data+buffer->leftLength+buffer->gapLength+1;

  /* Find next carrige return or the end of the buffer */
  while (*p++ != '\n' && p < buffer->data+buffer->length+buffer->gapLength);
  p--;
  moveGap(buffer,buffer->leftLength+(p-q));
}


void moveCursorUp(struct textBuffer *buffer)
{

  unsigned char *p,*q;
  int x;

  p=q=buffer->data+buffer->leftLength;

  /* Find previous carrige return */
  while ((*(p) != '\n' && p >= buffer->data)) p--;

  if (p > buffer->data) {
    p--;
    // Find previous carrige return
    while ((*(p) != '\n' && p >= buffer->data )) p--;
  }

  p++;
  // Set line position
  x=0;
  while (*p != '\n' && x < buffer->curx && p < buffer->data+buffer->length+buffer->gapLength) {
    if (*p == '\t') {
      x=x+_TABLENGTH-1;
      //x=x+(_TABLENGTH-(_TABLENGTH%x));
    } else {
      x++;
    }
    p++;
  }
  p--;

  moveGap(buffer,buffer->leftLength+(p-q));
  if (p > buffer->data)
  buffer->texty--;
}



void moveCursorDown(struct textBuffer *buffer)
{

  unsigned char *p,*q;
  int x;

  p=q=buffer->data+buffer->leftLength+buffer->gapLength+1;

  /* Find next carrige return or the end of the buffer */
  while (*p++ != '\n' && p < buffer->data+buffer->length+buffer->gapLength);

  /* Check for the end of the file */
  if (p < buffer->data+buffer->length+buffer->gapLength) {
    /* Set line position */
    x=1;
    while (*p != '\n' && x <= buffer->curx && p < buffer->data+buffer->length+buffer->gapLength) {
      if (*p == '\t') {
        x=x+_TABLENGTH-1;
        //x=x+(_TABLENGTH-(_TABLENGTH%x));
      } else {
        x++;
      }
      p++;
    }
    moveGap(buffer,buffer->leftLength+(p-q));
    buffer->texty++;
  }


}



int inGap(struct textBuffer *buffer, unsigned char *p)
{
  /* mind the gap! */
  //TODO: change values 1 (true) <-> 0 (false)
  if ((p > buffer->data+buffer->leftLength) && (p < buffer->data+buffer->leftLength+buffer->gapLength)) {
    return 0;
  } else {
    return 1;
  }
}


void insert(struct textBuffer *buffer, unsigned char ch)
{
  buffer->leftLength++;
  buffer->gapLength--;
  buffer->length++;
  *(buffer->data+buffer->leftLength)=ch;
  buffer->modified = 1;
  buffer->lastError[0] ='\0';
}

void delCurrentChar(struct textBuffer *buffer)
{
  buffer->leftLength--;
  buffer->gapLength++;
  buffer->length--;
  buffer->modified = 1;
}

void cursorLeft(struct textBuffer *buffer)
{
  if (buffer->leftLength > -1) {
    moveGap(buffer,buffer->leftLength-1);
  }
}

void cursorRight(struct textBuffer *buffer)
{
  if (buffer->leftLength < buffer->length-1) {
    moveGap(buffer,buffer->leftLength+1);
  }
}


/* get the pointers to the marks in the selected text */
void makePointersFromSelectedText(struct textBuffer *buffer, unsigned char **mark1, unsigned char **mark2)
{
  if (buffer->mark_init != buffer->mark_end && buffer->mark_end !=0) {
    // calculate pointers to marks
    if (buffer->leftLength >= buffer->mark_init) {
      *mark1 = buffer->data + buffer->mark_init;
    } else {
      *mark1 = buffer->data + buffer->gapLength + buffer->mark_init;
    }

    if (buffer->leftLength >= buffer->mark_end) {
      *mark2 = buffer->data + buffer->mark_end;
    } else {
      *mark2 = buffer->data + buffer->gapLength + buffer->mark_end;
    }
  }
}

void copy(struct textBuffer *buffer)
{
  unsigned char *mark1, *mark2, *tmp;
  int size, i;

  /* calculate buffer lenght */
  size = buffer->mark_init<buffer->mark_end?buffer->mark_end-buffer->mark_init:buffer->mark_init-buffer->mark_end;

  if (size > 0) {
    /* get memory for copy buffer */
    if (buffer->copybufferLength > 0) {
      /* free old buffer */
      free(buffer->copybuffer);
      buffer->copybufferLength = 0;
    }
    buffer->copybuffer = (unsigned char *)malloc(size);
    buffer->copybufferLength = size;

    makePointersFromSelectedText(buffer, &mark1, &mark2);

    /* copy data (except the gap) */
    i=0;
    while (mark1<=mark2) {
      if (!inGap(buffer, mark1)) {
        mark1 = buffer->data+buffer->leftLength+buffer->gapLength+1;
      }
      *(buffer->copybuffer+i++) = *mark1++;
    }
  }
}

void paste(struct textBuffer *buffer)
{
  unsigned char *ch;
  int i;

  for (i=0; i<buffer->copybufferLength; i++) {
    buffer->leftLength++;
    buffer->gapLength--;
    buffer->length++;
    *(buffer->data+buffer->leftLength)=*(buffer->copybuffer + i);
  }
  buffer->modified = 1;
  buffer->lastError[0] ='\0';
}

void delselected(struct textBuffer *buffer)
{
  unsigned char *tmp;
  int mark1, mark2;
  int i, j, k;

  //TODO: remove order control
  if (buffer->mark_init < buffer->mark_end) {
    mark1 = buffer->mark_init;
    mark2 = buffer->mark_end;
  } else {
    mark2 = buffer->mark_init;
    mark1 = buffer->mark_end;
  }

  /* allocate memory for the new buffer */
  tmp = (unsigned char *)malloc(buffer->size);

  /* copy data (except the gap and selected text) */
  i=0;
  j=0;
  k=0;
  while (j < buffer->size) {
    if (!inGap(buffer, buffer->data+j)) {
      buffer->leftLength = i-1;
      j = j + buffer->gapLength-1;
      i = i + buffer->gapLength-1;
    }
    /* don't copy selected text */
    if (k < mark1 || k > mark2) {
      *(tmp + i++) = *(buffer->data + j);
    }
    j++;
    k++;
  }
  buffer->length = buffer->length - (buffer->mark_end - buffer->mark_init);
  buffer->mark_init = buffer->mark_end = 0;
  //TODO: assign b->data to tmp instead of copy?
  memcpy(buffer->data, tmp, buffer->size);
  free(tmp);
}
