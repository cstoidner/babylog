
#ifndef __BABYLOG_H__
#define __BABYLOG_H__

char *babylog_getLog();
int   babylog_getStatus();
void  babylog_reset();

int   babylog_printf(const char *format, ...);

/* easy output function for one value with name; those might 
   allow to avoid complex vsnprintf() (e.g. im boot-code) */
/*
int   babylog_putNamed_i(const char *name, int i); // integer value in decimal format
int   babylog_putNamed_h(const char *name, int h); // integer value in hex format
int   babylog_putNamed_s(const char *name, const char* s); // string value
*/



#ifdef BABYLOG_HAVE_IMPLEMENTATION

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifndef BABYLOG_BUFSIZE
  #define BABYLOG_BUFSIZE 128 /* in bytes (including string's term-zero) */
#endif

/* string to put into the buffer to keep track that overflow has occurred */
static const char BABYLOG_OVERFLOW_MARKER[] = "\n***LOG BUFFER OVERFLOW***\n";

/* buffer to hold all log-messages; have the buffer larger by size of overflow-marker
   to have always the chance to put the marker into the buffer */
static char babylog_buf[BABYLOG_BUFSIZE + sizeof(BABYLOG_OVERFLOW_MARKER)] = {0};
static int  babylog_bufLen = 1; /* bytes in buffer, including term-zero */
static int  babylog_status = 0; /* 0=normal operation, -1=some error occured at some place */


int babylog_printf(const char *format, ...)
{
  va_list args;
  int bufsizeLeft = BABYLOG_BUFSIZE - babylog_bufLen;
  int nlogged;
  int res;

  if (babylog_status<0)
  {
    /* the log-message before has failed, dont put newer log-messages
       into the buffer to keep transparent which message has failed */
    return babylog_status;
  }

  if (bufsizeLeft <= 0)
  {
  	/* buffer is already full */
    babylog_status = -2; /* keep notice of overflow-error */
    return babylog_status;
  }

  va_start (args, format);
  /* start at position of term-zero */
  res = vsnprintf(&babylog_buf[babylog_bufLen-1], bufsizeLeft, format, args);
  va_end (args);

  if (res >= bufsizeLeft)
  {
  	/* log-message was truncated since buffer is full */
    babylog_bufLen = BABYLOG_BUFSIZE; /* mark buffer as full */
    babylog_buf[BABYLOG_BUFSIZE-1] = '\0'; /* add trailing-zero to be in the safe */ 
    babylog_status = -2; /* keep notice of er overflow-error */
    strcat(babylog_buf, BABYLOG_OVERFLOW_MARKER); /* no size check needed, since buffer fits always for marker */
  }
  else if (res < 0)
  {
  	/* some error occured */
    babylog_status = -1; /* keep notice of unknown error */
  }
  else
  {
  	/* log-message was succesfully added to buffer */
    babylog_bufLen += res; /* increase buffer by message-size */
  }

  return babylog_status;
}

char *babylog_getLog()
{
	return babylog_buf;
}

int babylog_getStatus()
{
	return babylog_status;
}

void  babylog_reset()
{
  /* empty the buffer and clear the status */
  babylog_buf[0] = '\0';
  babylog_bufLen = 1;
  babylog_status = 0;
}

#endif /* BABYLOG_HAVE_IMPLEMENTATION */


#endif /* __BABYLOG_H__ */
