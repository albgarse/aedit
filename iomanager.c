#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "datatypes.h"
#include "iomanager.h"
#include "datamanager.h"


int load(struct textBuffer *buffer, char *file)
{

 int fd, filesize,size;
 struct stat fstate;

	if ((fd = open(file,O_RDONLY)) < 0) {
		return _KO;
	}

	if (fstat(fd,&fstate) < 0) {
		return _KO;
	}

	filesize = ((int)fstate.st_size);
	size = filesize + _INIT_SIZE;


	if (createBuffer(buffer,size) == _OK) {
		if (read(fd,buffer->data,filesize) < 0) {
			return _KO;
		}

		buffer->length = filesize;
		/* move the gap to end of file */
		buffer->leftLength=filesize;
		buffer->gapLength=size-filesize;
		return _OK;

    close(fd);

	} else {
		return _KO;
	}
}

int save(struct textBuffer *buffer,char *file)
{
  int fd,writtenbytes;

  if ((fd = open(file,O_WRONLY)) < 0) {
    return _KO;
  }
  /* save left portion of gap buffer file */
  if (write(fd, buffer->data, buffer->leftLength) < 0) {
    return _KO;
  }
  /* save right portion of gap buffer file */
  if (write(fd, buffer->data + buffer->leftLength + buffer->gapLength, buffer->length - buffer->leftLength) < 0) {
    return _KO;
  }

  buffer->modified = 0;
  close(fd);
  return _OK;
}
