

/* Copyright (C) 1998-2002 Chancelier Jean-Philippe */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifndef __STDC__
#include <malloc.h>
#endif
#include <stdio.h>

#include "../machine.h"

#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"


static int Sed __PARAMS ((int, char *, FILE *, char *, char *, char *, char *, char *, char *));
static void readOneLine __PARAMS ((char *buff, int *stop, FILE * fd));
static void ConvertName __PARAMS ((char *filein, char *fileout));
extern void sciprint __PARAMS ((char *fmt,...));

/**************************************************
 * Converts a scilab Eps file to an Epsf file 
 * by adding a preamble 
 **************************************************/

static char entete[256];

int ScilabPsToEps (char orientation, char *filein, char *fileout)
{
  int flag = 0, rep;
  FILE *fo;
  char *env;
  env = getenv ("SCI");
  if (env == NULL)
    {
      sciprint (MSG_ERROR49);
      return (1);
    }
  sprintf (entete, "%s/imp/NperiPos.ps", env);

  ConvertName (filein, fileout);
  if (strcmp (fileout, filein) == 0)
    {
      flag = 1;
      strcat (fileout, ".temp");
    }

  if ((fo = fopen (fileout, "w")) == 0)
    {
      sciprint (MSG_ERROR50, fileout);
      return 1;
    }

  fprintf (fo, "%%!PS-Adobe-2.0 EPSF-2.0\n");
  if (orientation == 'p')
    fprintf (fo, "%%%%BoundingBox: 0 200 600 624\n");
  else
    fprintf (fo, "%%%%BoundingBox: 0 0 600 780\n");

  Sed (0, entete, fo, "%!PS-ADOBE", "%%", (char *) 0, (char *) 0, (char *) 0, (char *) 0);

  if (orientation == 'p')
    rep = Sed (1, filein, fo, "[0.5 10 div 0 0 0.5 10 div neg  0 2120 10 div] concat",
	       "[0.5 5 div 0 0 0.5 5 div neg  0 3120 5 div] concat",
	       (char *) 0, (char *) 0, (char *) 0, (char *) 0);
  else
    rep = Sed (1, filein, fo, "[0.5 10 div 0 0 0.5 10 div neg  0 2120 10 div] concat",
	       "90 rotate 10 640 neg translate [0.5 5 div 0 0 0.5 5 div neg  0 3120 5 div] concat",
	       (char *) 0, (char *) 0, (char *) 0, (char *) 0);
  fclose (fo);

  if (rep >= 1)
    {
      if (rep == 1)
	sciprint (MSG_ERROR51);
      remove (fileout);
      return (0);
    }

  if (flag == 1)
    {
/** we move xxx.temp to xxx */
      fo = fopen (filein, "w");
      Sed (0, fileout, fo, (char *) 0, (char *) 0, (char *) 0, (char *) 0, (char *) 0, (char *) 0);
      fclose (fo);
    }
  else
    {
      remove (filein);
    }
  return (0);
}


static void 
ConvertName (filein, fileout)
     char *filein, *fileout;
{
  char *p = filein, *p1;
  p1 = strchr (p, '/');
  while (p1 != 0)
    {
      p = p1 + 1;
      p1 = strchr (p, '/');
    }
  p = strchr (p, '.');
  if (p != 0)
    {
      *p = '\0';
      sprintf (fileout, "%s.eps", filein);
      *p = '.';
    }
  else
    sprintf (fileout, "%s.eps", filein);
/** sciprint("[%s]=>[%s]\r\n",filein,fileout); **/
}


/**************************************************
 * copies file to fileo performing some substitutions 
 **************************************************/

static int 
Sed (flag, file, fileo, strin1, strout1, strin2, strout2, strin3, strout3)
     char file[], strin1[], strout1[], strout3[];
     char strin2[], strout2[], strin3[];
     FILE *fileo;
     int flag;
{
  FILE *fd;
  fd = fopen (file, "r");
  if (fd != 0)
    {
      int stop = 0;
      while (stop != 1)
	{
	  char buff[512];
	  readOneLine (buff, &stop, fd);
	  if (flag == 1)
	    {
	      if (strncmp (buff, "%!PS-Adobe-2.0 EPSF-2.0",
			   strlen ("%!PS-Adobe-2.0 EPSF-2.0")) == 0)
		{
		  fclose (fd);
		  return (1);
		}
	    }
	  if (strin1 != (char *) 0 && strncmp (buff, strin1, strlen (strin1)) == 0)
	    fprintf (fileo, "%s\n", strout1);
	  else
	    {
	      if (strin2 != (char *) 0 && strncmp (buff, strin2, strlen (strin2)) == 0)
		fprintf (fileo, "%s\n", strout2);
	      else
		{
		  if (strin3 != (char *) 0 && strncmp (buff, strin3, strlen (strin3)) == 0)
		    fprintf (fileo, "%s\n", strout3);
		  else
		    fprintf (fileo, "%s", buff);
		}
	    }
	}
      fclose (fd);
    }
  else
    {
      sciprint (MSG_ERROR52, file);
      return (2);
    }
  return (0);
}

/*-----------------------------------------------
  lit une ligne dans fd et la stocke dans buff
---------------------------------------------------*/

static void 
readOneLine (buff, stop, fd)
     char buff[];
     int *stop;
     FILE *fd;
{
  int i, c;
  for (i = 0; (c = getc (fd)) != '\n' && c != EOF; i++)
    buff[i] = c;
  buff[i] = '\n';
  buff[i + 1] = '\0';
  if (c == EOF)
    {
      *stop = 1;
    }
}
