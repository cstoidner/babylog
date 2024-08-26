
#ifndef __BABYLOG_H__
#define __BABYLOG_H__

char *babylog_getLog(void);
int   babylog_getStatus(void);
void  babylog_reset(void);

#ifndef BABYLOG_NO_VSNPRINTF
int   babylog_printf(const char *format, ...);
#endif
int   babylog_puts(const char *s);
/* easy output function for one value with name; those might 
   allow to avoid complex vsnprintf() (e.g. im boot-code) */
/*
int   babylog_putNamed_i(const char *name, int i); // integer value in decimal format
int   babylog_putNamed_x(const char *name, int h); // integer value in hex format
int   babylog_putNamed_s(const char *name, const char* s); // string value
*/
int   babylog_putNamed_lx(const char *name, long x); // long integer value in hex format

static char * ltoa(long value, int radix);

#ifdef BABYLOG_HAVE_IMPLEMENTATION

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifndef BABYLOG_BUFSIZE
  #define BABYLOG_BUFSIZE 1024 /* in bytes (including string's term-zero) */
#endif

/* string to put into the buffer to keep track that overflow has occurred */
static const char BABYLOG_OVERFLOW_MARKER[] = "\n***LOG BUFFER OVERFLOW***\n";

/* buffer to hold all log-messages; have the buffer larger by size of overflow-marker
   to have always the chance to put the marker into the buffer */
static char babylog_buf[BABYLOG_BUFSIZE + sizeof(BABYLOG_OVERFLOW_MARKER)] = {0};
static int  babylog_bufLen = 1; /* bytes in buffer, including term-zero */
static int  babylog_status = 0; /* 0=normal operation, -1=some error occured at some place */

int babylog_puts(const char *s)
{
  int bufsizeLeft = BABYLOG_BUFSIZE - babylog_bufLen;
  int len = strlen(s);

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

  strncpy(&babylog_buf[babylog_bufLen-1], s, bufsizeLeft-1);

  if (len >= bufsizeLeft)
  {
    /* string must be truncated since buffer is full */
    babylog_bufLen = BABYLOG_BUFSIZE; /* mark buffer as full */
    babylog_buf[BABYLOG_BUFSIZE-1] = '\0'; /* add trailing-zero to be in the safe */
    babylog_status = -2; /* keep notice of er overflow-error */
    strcat(babylog_buf, BABYLOG_OVERFLOW_MARKER); /* no size check needed, since buffer fits always for marker */
  }
  else
  {
    /* log-message was succesfully added to buffer */
    babylog_bufLen += len; /* increase buffer by message-size */
    babylog_buf[babylog_bufLen] = '\0'; /* assure having trailing zero */
  }

  return babylog_status;
}

int   babylog_putNamed_lx(const char *name, long x) // long integer value in hex format
{
  babylog_puts(name);
  babylog_puts(": ");
  babylog_puts(ltoa(x, 0x10));
  babylog_puts("\n");
  return babylog_status;
}

#ifndef BABYLOG_NO_VSNPRINTF
int babylog_printf(const char *format, ...)
{
  va_list args;
  int bufsizeLeft = BABYLOG_BUFSIZE - babylog_bufLen;
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
#endif

char *babylog_getLog(void)
{
	return babylog_buf;
}

int babylog_getStatus(void)
{
	return babylog_status;
}

void  babylog_reset(void)
{
  /* empty the buffer and clear the status */
  babylog_buf[0] = '\0';
  babylog_bufLen = 1;
  babylog_status = 0;
}

static char * ltoa(long value, int radix)
{
  char tmp[21+1]; // just prepared for 64bit,
                  // longest value: "0xffffffffffffffff"
                  //            or: "-18446744073709551615"
                  // i.E. we need 21 character + '\0'
  static char return_buffer[21+1];
  char *sp = return_buffer;
  char *tp = tmp;
  int i;
  unsigned v;

  int sign = (radix == 10 && value < 0);
  if (sign)
        v = -value;
  else
        v = (unsigned)value;

  while (v || tp == tmp)
  {
        i = v % radix;
        v /= radix;
        if (i < 10)
          *tp++ = i+'0';
        else
          *tp++ = i + 'a' - 10;
  }

  int len = tp - tmp;

  if (radix == 0x10)
  {
      *sp++ = '0';
      *sp++ = 'x';
  }
  else if (sign) 
  {
      *sp++ = '-';
      len++;
  }

  while (tp > tmp)
        *sp++ = *--tp;

  return return_buffer;
}

#endif /* BABYLOG_HAVE_IMPLEMENTATION */


#endif /* __BABYLOG_H__ */
