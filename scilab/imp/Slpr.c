/* Copyright ENPC/Chancelier Jean-Philippe */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifndef __STDC__
#include <malloc.h>
#endif 
#include <stdio.h>

#if defined(netbsd)
#include <ieeefp.h>
#endif

#if defined(freebsd)
#include <floatingpoint.h>
#endif

#include "util.h" 

static void Sed (char *,char *,char *,char *,char *,char *,char *);
static void ComputeSize (int num,int i,double *,double *,double *,double *);

#ifdef WIN32 
extern void SciEnv(void);
#endif 

/*---------------------------------------------------------

  Blpr str file1 .... filen  | lpr 
                             >! foo.ps 

  fait une mise en page de n dessins scilab ou neoclo 

  avec un titre str et genere le fichier postscript associe
-------------------------------------------------------------*/

char * UsageStr[]={
  "Usage  : Blpr Title file1 ... filen | lpr   \n",
  "       : Blpr Title file1 ... filen > fileres  \n",
  "\tfile1, ... filen : are n Postscript files produced by Scilab\n",
  "\tfileres : a file name for the result \n",
  "\ttitle : a string for title\n",
  "\tThis command print n Postscript files produced by Scilab on a\n",
  "\tsingle page with a title Title\n",
  "fin"};

char entete[160];

int main(int argc, char **argv)
{
  char *env;
  double x,y,w,h;
  char buf[256];
  int i ;
  FILE *fd;
#if defined(freebsd) || defined(netbsd)
  fpsetmask(0);
#endif
  if (argc <= 2) { int i=0;
		   while (strcmp(UsageStr[i],"fin")!=0)
		     { 
		       fprintf(stderr,"%s",UsageStr[i]),i++;
		     }
		   exit(0);
		 }
#ifdef WIN32 
  SciEnv();
#endif 
  env = getenv("SCI");
  if (env == NULL) {
    fprintf(stderr,"Environment variable SCI must be defined\n");
    exit(0);
  }
  sprintf(entete,"%s/imp/NperiPos.ps",env);
  fd=fopen(entete,"r");
  if (fd != 0)
    { int c;
      while ( (c=getc(fd)) != EOF)  putc((char) c,stdout);
      fclose(fd);
    }
  else 
    {
      fprintf(stderr,"file %s not found ",entete);
      return(1);
    }
  fprintf(stdout,"/Times-Roman findfont 14 scf mul scalefont setfont\n");
  fprintf(stdout,"100 780 moveto ( %s ) show\n",argv[1]);
  for ( i = 2 ; i < argc-1 ; i++)
    { 
      ComputeSize(argc-2,i-1,&x,&y,&w,&h)      ;
      sprintf(buf,"gsave [1 0 0 -1 0 0] concat %5.2f %5.2f %5.2f %5.2f DesPosi"
	      ,x,y,w,h);
      Sed(argv[i],"[0.5 10 div 0 0 0.5 10 div neg  0 2120 10 div] concat",
	  buf," showpage","grestore",
	  " end saved restore","% end saved restore");
    }
  ComputeSize(argc-2,argc-2,&x,&y,&w,&h);      
  sprintf(buf,"gsave [1 0 0 -1 0 0] concat %5.2f %5.2f %5.2f %5.2f DesPosi"
	  ,x,y,w,h);
  Sed(argv[argc-1],"[0.5 10 div 0 0 0.5 10 div neg  0 2120 10 div] concat",
      buf, " showpage"," grestore showpage",
      " end saved restore"," end saved restore");
  return(0);
}


/*---------------------------------------------------
 remplace strin<i> par strout<i> en lisant le contenu de 
 file et en ecrivant sur stdout
-------------------------------------------------------*/

static void Sed(char *file, char *strin1, char *strout1, char *strin2, char *strout2, char *strin3, char *strout3)
{
  FILE *fd;

  static char *buff = NULL;
  static int buflen = 512;
  if ( buff == NULL) 
    {
      buff = malloc(buflen*sizeof(char));
      if ( buff == NULL) 
	{
	  fprintf(stderr,"Running out of space \n");
	  exit(1);
	}
    }

  fd=fopen(file,"r");
  if (fd != 0)
    { int stop=0;
      while ( stop != 1)
	{  
	  read_one_line (&buff,&stop,fd,&buflen); 
	   if (strncmp(buff,strin1,strlen(strin1))==0)
	     fprintf(stdout,"%s\n",strout1);
	   else
	     {
	       if (strncmp(buff,strin2,strlen(strin2))==0)
		 fprintf(stdout,"%s\n",strout2);
	       else 
		 {
		   if (strncmp(buff,strin3,strlen(strin3))==0)
		     fprintf(stdout,"%s\n",strout3);
		   else
		     fprintf(stdout,"%s",buff);
		 }
	     }
	 }
      fclose(fd);
    }
  else 
    {
      fprintf(stderr,"file %s not found ",file);
      return;
    }
}



/*-----------------------------------------------
  calcule la taille pour un dessin suivant le nombre de dessin a 
  placer dans la feuille 
  num est le nombre de dessins et i le numero du dessin 
  renvoit le point gauche dans (*x,*y) la largeur et le hauteur 
  dans (*w,*h)
-----------------------------------------------------*/

static void ComputeSize(int num, int i, double *x, double *y, double *w, double *h)
{
  switch (num)
    {
    case 1 :
      *x=3.0;*y=24.0;*h=20.0;*w=15.0;
      break;
    case 2 :
      *x=3.0;*y=(24-(i-1)*11);*h=10.0;*w=15.0;
      break;
    case 3 :
      *x=3.0;*y=(26.5-(i-1)*8.5);*h=8.0;*w=15.0;
      break;
    case 4 :
      if (i <= 2) *y=24.0;else *y=24-11.0;
      if ( (i % 2 ) == 0 ) 
	{ *x= 2+8.5;*h=8.0;*w=8.0;}
      else 
	{*x= 2.0;*h=8.0;*w=8.0;}
      break ;
    case 5 :
    case 6 :
      if (i <= 2) *y=26.0;else { if ( i <= 4) *y=26-8.5; else *y=26-2*8.5;}
      if ( (i % 2 ) == 0 ) 
	{ *x= 2+8.5;*h=7.5;*w=8.0;}
      else 
	{*x= 2.0;*h=7.5;*w=8.0;}
      break ;
    case 7 :
    case 8 :
    case 9 :
      if (i <= 3) *y=26.0;else { if ( i <= 6) *y=26-8.5; else *y=26-2*8.5;}
      if ( (i % 3 ) == 0 ) 
	{ *x= 1.5+2*6;*h=7.5;*w=5.5;}
      else 
	{
	  if ( (i % 3 ) == 1 ) 
	    { *x= 1.5;*h=7.5;*w=5.5;}
	  else 
	    {*x= 1.5+6 ;*h=7.5;*w=5.5;}
	}
      break ;
    case 10 :
    case 11 :
    case 12 :
    default :
      if (i <= 3) *y=27.0;
      else { 
	if ( i <= 6) *y=27-6.5;
	else {
	  if (i <= 9 ) *y=27-2*6.5; else *y=27-3*6.5;}
      }
      if ( (i % 3 ) == 0 ) 
	{ *x= 1.5+2*6;*h=6;*w=5.5;}
      else 
	{
	  if ( (i % 3 ) == 1 ) 
	    { *x= 1.5;*h=6.0;*w=5.5;}
	  else 
	    {*x= 1.5+6 ;*h=6.0;*w=5.5;}
	}
      break ;
    }
}


