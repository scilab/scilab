
/* Copyright (C) 1998-2002 Chancelier Jean-Philippe */

#include <stdio.h>
#include <stdlib.h>
extern char *readline_win (char * , int);

main ()
{
  int i;
  char *line;
  for (i = 0; i < 10; i++)
    {
      line = readline_win ("test",0);
      if (line != (char *) 0)
	{
	  printf ("line %s\r\n", line);
	}
    }
}


/****************************************************************
 *alloc:
 * allocate memory 
 * This is a protected version of malloc. It causes an int_error 
 * if there is not enough memory. If message is NULL, we 
 * allow NULL return. Otherwise, we handle the error, using the
 * message to create the int_error string. Note cp/sp_extend uses realloc,
 * so it depends on this using malloc().
 *****************************************************************/

char *
alloc (size, message)
     unsigned long size;	/* # of bytes */
     char *message;		/* description of what is being allocated */
{
  char *p;			/* the new allocation */
  char errbuf[100];		/* error message string */
  p = malloc ((size_t) size);	/* try again */
  if (p == (char *) NULL)
    {
      /* really out of memory */
      if (message != NULL)
	{
	}
    }
  return (p);
}
