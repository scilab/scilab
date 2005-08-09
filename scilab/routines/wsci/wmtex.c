/* Copyright (C) 1998-2002 Chancelier Jean-Philippe */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "../machine.h"

#include "wcommon.h" 

#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"


static int Sed __PARAMS ((int, char *, FILE *, char *, char *, char *, char *, char *, char *));
static void readOneLine __PARAMS ((char *buff, int *stop, FILE * fd));
static void FileNameChange __PARAMS ((char *filein, char *fileout, char *, char *));

/**************************************************
 * Converts a scilab Eps file to an Epsf file 
 * by adding a preamble and produces a tex file
 **************************************************/

static char entete[256], base[54];

int ScilabPsToTeX (char orientation, char *filein, char *fileout, double xs, double ys)
{
  double wide, high, widecm, highcm, hscale, vscale;
  int flag = 0, rep;
  FILE *fo;
  char *env;
  env = getenv ("SCI");
  if (env == NULL)
    {
      sciprint (MSG_ERROR49);
      return (1);
    }

  /** first we produce the epsf file from the scilab file 
    without preamble **/

  sprintf (entete, "%s/imp/NperiPos.ps", env);

  FileNameChange (filein, fileout, base, "eps");
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
    fprintf (fo, "%%%%BoundingBox:  0 0 300 212\n");
  else
    fprintf (fo, "%%%%BoundingBox:  0 0 212 300\n");

  Sed (0, entete, fo, "%!PS-ADOBE", "%%", (char *) 0, (char *) 0, (char *) 0, (char *) 0);

  if (orientation == 'p')
    rep = Sed (1, filein, fo, (char *) 0, (char *) 0, (char *) 0, (char *) 0,
	       (char *) 0, (char *) 0);
  else
    rep = Sed (1, filein, fo, "[0.5 10 div 0 0 0.5 10 div neg  0 2120 10 div] concat",
	       "90 rotate 0 600 neg translate [0.5 10 div 0 0 0.5 10 div neg  0 3000 5 div] concat",
	       (char *) 0, (char *) 0, (char *) 0, (char *) 0);
  fclose (fo);

  if (rep >= 1)
    {
      if (rep == 1)
	sciprint (MSG_ERROR51);
      remove (fileout);
      return 0;
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

/** and now the TeX file **/

  if (orientation == 'p')
    {
      wide = 300 * xs;
      high = 212 * ys;
    }
  else
    {
      wide = 212 * xs;
      high = 300 * ys;
    }
  widecm = wide * 2.835;
  highcm = high * 2.835;
  hscale = xs * 100;
  vscale = ys * 100;

  FileNameChange (filein, fileout, base, "tex");
  if ((fo = fopen (fileout, "w")) == 0)
    {
      sciprint (MSG_ERROR51, fileout);
      return 1;
    }
#ifndef DOC

  fprintf (fo, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
  fprintf (fo, MSG_SCIMSG46);
  fprintf (fo, MSG_SCIMSG47);
  fprintf (fo, MSG_SCIMSG48);
  fprintf (fo, MSG_SCIMSG49);
  fprintf (fo, MSG_SCIMSG50);
  fprintf (fo, MSG_SCIMSG51);
  fprintf (fo, "%%          \n");
  fprintf (fo, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");

  fprintf (fo, " \n");


  fprintf (fo, " \\long\\def\\Checksifdef#1#2#3{%%\n");
  fprintf (fo, "\\expandafter\\ifx\\csname #1\\endcsname\\relax#2\\else#3\\fi}\n");
  fprintf (fo, "\\Checksifdef{Figdir}{\\gdef\\Figdir{}}{}\n");
  fprintf (fo, "\\def\\dessin#1#2{\n");
#endif
  fprintf (fo, "\\begin{figure}[hbtp]\n");
  fprintf (fo, "\\begin{center}\n");
  fprintf (fo, MSG_SCIMSG52);
  fprintf (fo, "%%\\setlength{\\unitlength}{1mm}\n");
  fprintf (fo, "%%\\fbox{\\begin{picture}(%.2f,%.2f)\n", widecm, highcm);
  fprintf (fo, "\\fbox{\\begin{picture}(%.2f,%.2f)\n", wide, high);
  FileNameChange (filein, fileout, base, "eps");
#ifdef EPSFIG
  fprintf (fo, "\\epsfig{file=\\Figdir %s.eps,width=%.2fpt,height=%.2fpt}\n",
	   base, wide.high);
#else
  fprintf (fo, "%% if you want to use epsfig uncomment the following line \n");
  fprintf (fo, "%% and comment the special line \n");
  fprintf (fo, "%%\\epsfig{file=\\Figdir %s.eps,width=%.2fpt,height=%.2fpt}\n",
	   base, wide, high);
  fprintf (fo, "\\special{psfile=\\Figdir %s.eps hscale=%.2f vscale=%.2f}\n",
	   base, hscale, vscale);
#endif
  fprintf (fo, "\\end{picture}}\n");
  fprintf (fo, "\\end{center}\n");
#ifndef DOC
  fprintf (fo, "\\caption{\\label{#2}#1}\n");
  fprintf (fo, "\\end{figure}}\n");
#endif
  fclose (fo);
  return (0);
}

/*****************************************
 * xxx/xxx/xxx/ffff.zzz -->  ffff.syf 
 * and name = "ffff"
 ****************************************/

static void 
FileNameChange (filein, fileout, name, suf)
     char *filein, *fileout, *name, *suf;
{
  char *p = filein, *p1;
  p1 = strchr (p, '/');
  while (p1 != 0)
    {
      p = p1 + 1;
      p1 = strchr (p, '/');
    }
  p1 = strchr (p, '.');
  if (p1 != 0)
    {
      *p1 = '\0';
      sprintf (fileout, "%s.%s", filein, suf);
      sprintf (base, "%s", p);
      *p1 = '.';
    }
  else
    {
      sprintf (fileout, "%s.%s", filein, suf);
      sprintf (base, "%s", p);
    }
  /* 
     sciprint("[%s]=>[%s]\n",filein,fileout); 
     sciprint("real base %s\n",base);
   */
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
