/* Copyright ENPC/Chancelier Jean-Philippe */

#ifdef __STDC__
# include <stdlib.h>
# include <stdarg.h>
#else
# include <varargs.h>
# include <malloc.h>
char *getenv();
#endif 

#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "../routines/machine.h"
#include "util.h" 

static int Sed __PARAMS((int,char *,FILE *,char *,char *,char *,char *,char *,char *));

static void FileNameChange __PARAMS((char *filein,char *fileout,char *,char *));
static  void dos2win32 __PARAMS((char *filename,char *filename1));
int ScilabPsToTeX __PARAMS((char orientation,char *filenamein,char *filenameout,double xs,double ys));

/**************************************************
 * S;latexpr  Usage :  [-orientation] xs ys filename.ps 
 * used to produce an EPSF file from a scilab postscript file 
 * without Postscript preamble
 **************************************************/

char * UsageStr[]={
  "\tfilename.ps : Postscript file produced by Scilab\n",
  "\torientation : p[ortrait] or l[andscape]  \n",
  "fin"};

char file1[256],file2[256];

#ifdef WIN32 
extern void SciEnv(void);
#endif 

int main(int argc, char **argv)
{
  double xs=1.0,ys=1.0;
  char orientation='p';
#ifdef WIN32 
  SciEnv();
#endif 
  if (argc >= 6 || argc <= 3 )
    { int i=0;
      fprintf(stderr,"Usage  : %s [-orientation] xs ys filename.ps \n",argv[0]);
      while (strcmp(UsageStr[i],"fin")!=0)
	  fprintf(stderr,"%s",UsageStr[i]),i++;
      return(1);
    }
  switch ( argc ) 
    {
    case 4 :
      if ( strncmp( argv[1] ,"-p",2) == 0 || ( strncmp( argv[1] ,"-l",2) == 0) )
	{
	  fprintf(stderr,"Slatexpr: wrong number of arguments \n");
	  return(1);
	}
      orientation = 'p';
      strcpy(file1,argv[3]);
      sscanf(argv[1],"%lf",&xs);
      sscanf(argv[2],"%lf",&ys);
      break;
    case 5:
      strcpy(file1,argv[4]);
      if ( strncmp( argv[1] ,"-p",2) == 0) 
	orientation = 'p';
      else if ( strncmp( argv[1] ,"-l",2) == 0) 
	orientation = 'l';
      else 
       fprintf(stderr,"Invalid first argument %s\n",argv[1]);
      sscanf(argv[2],"%lf",&xs);
      sscanf(argv[3],"%lf",&ys);
      break;
    }
  /** converts file1 format into file2 **/
  dos2win32(file1,file2);
  /** file2 : input file , file1 : output file **/
  ScilabPsToTeX(orientation,file2,file1,xs,ys);
  return(0);
}


#ifdef __STDC__

void sciprint(char *fmt, ...)
{
  va_list args;
  va_start(args,fmt);
  vfprintf(stdout, fmt, args );
  va_end(args);
}
#else 

/*VARARGS0*/
void sciprint(va_alist) va_dcl
{
  va_list ap;
  char *format;
  va_start(ap);
  format = va_arg(ap, char *);
  (void)  vfprintf(stdout, format, ap );
  va_end(ap);
}
#endif 


static  void dos2win32(char *filename, char *filename1)
{
#ifdef CVT_PATH_BEG
  if ( filename[1] == ':' ) 
    {
      *filename1++ = '/';
      *filename1++ = '/';
      *filename1++ = *filename++;
      filename++;
    }
#endif
  while ( *filename != '\0' ) 
    {
      *filename1++ = *filename++;
      if ( *(filename1-1) == '\\' ) *(filename1-1) = '/' ;
    }
  *filename1 = '\0';
}

/** file wmtex.c in wsci ( should be gathered in a unique 
  lib ) **/

/**************************************************
 * Converts a scilab Eps file to an Epsf file 
 * by adding a preamble and produces a tex file
 **************************************************/

static char entete[256],base[54];

int ScilabPsToTeX(char orientation, char *filein, char *fileout, double xs, double ys)
{
  double wide,high,widecm,highcm,hscale,vscale;
  int flag = 0,rep;
  FILE *fo;
  char *env;
  env = getenv("SCI");
  if (env == NULL) 
    {
      sciprint("Environment variable SCI must be defined\n");
      return(1);
    }

  /** first we produce the epsf file from the scilab file 
    without preamble **/

  sprintf(entete,"%s/imp/NperiPos.ps",env);

  FileNameChange(filein,fileout,base,"eps");
  if ( strcmp(fileout,filein) == 0 ) 
    {
      flag = 1;
      strcat(fileout,".temp");
    }
  
  if ( (fo = fopen(fileout,"w"))== 0 ) 
    {
      sciprint(" Can't open file %s\n",fileout);
      exit(1);
    }

  fprintf(fo,"%%!PS-Adobe-2.0 EPSF-2.0\n");
  if ( orientation == 'p' ) 
    fprintf(fo,"%%%%BoundingBox:  0 0 300 212\n");
  else 
    fprintf(fo,"%%%%BoundingBox:  0 0 212 300\n");

  Sed(0,entete,fo,"%!PS-ADOBE","%%",(char*) 0,(char *)0,(char*) 0,(char *)0);

  if ( orientation == 'p' )
        rep=Sed(1,filein,fo,(char*) 0,(char *)0,(char*) 0,(char *)0,
		(char*) 0,(char *)0);
  else 
    
    rep=Sed(1,filein,fo,"[0.5 10 div 0 0 0.5 10 div neg  0 2120 10 div] concat",
	"90 rotate 0 600 neg translate [0.5 10 div 0 0 0.5 10 div neg  0 3000 5 div] concat",
	(char*) 0,(char *)0,(char*) 0,(char *)0);
  fclose(fo);

  if ( rep >= 1 ) 
    {
      if ( rep == 1) 
	sciprint("input file doesn't need to be converted to Epsf\n");
      remove(fileout);
      exit(0);
    }
  
  if ( flag == 1) 
    {
      /** we move xxx.temp to xxx */
      fo = fopen(filein,"w");
      Sed(0,fileout,fo,(char*) 0,(char *)0,(char*) 0,(char *)0,(char*) 0,(char *)0);
      fclose(fo);
    }
  else
    {
      remove(filein);
    }
  
  /** and now the TeX file **/

  if ( orientation == 'p' ) 
    {
      wide = 300 *xs;
      high = 212 *ys;
    }
  else 
    {
      wide = 212 *xs;
      high = 300 *ys;
    }
  widecm = wide *2.835;
  highcm = high *2.835;
  hscale = xs*100;
  vscale = ys*100;

  FileNameChange(filein,fileout,base,"tex");
  if ( (fo = fopen(fileout,"w"))== 0 ) 
    {
      sciprint(" Can't open file %s\n",fileout);
      exit(1);
    }
#ifndef DOC
  fprintf(fo,"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
  fprintf(fo,"%% Usage: -To include a Figure with a caption, insert the TWO following lines\n");
  fprintf(fo,"%%        in your Latex file:\n");
  fprintf(fo,"%% \\input{This_file_name} \n");
  fprintf(fo,"%% \\dessin{The_caption}{The_label}\n");
  fprintf(fo,"%%         -To include just a picture, insert the lines \n");
  fprintf(fo,"%%         between \\fbox{\\begin{picture}...  and \\end{picture}} below \n");
  fprintf(fo,"%%          \n");
  fprintf(fo,"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");

  fprintf(fo," \n");

  fprintf(fo," \\long\\def\\Checksifdef#1#2#3{%%\n");
  fprintf(fo,"\\expandafter\\ifx\\csname #1\\endcsname\\relax#2\\else#3\\fi}\n");
  fprintf(fo,"\\Checksifdef{Figdir}{\\gdef\\Figdir{}}{}\n");
  fprintf(fo,"\\def\\dessin#1#2{\n");
#endif
  fprintf(fo,"\\begin{figure}[hbtp]\n");
  fprintf(fo,"\\begin{center}\n");
  fprintf(fo,"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% \n");
  fprintf(fo,"%%If you prefer cm, uncomment the following two lines\n");
  fprintf(fo,"%%\\setlength{\\unitlength}{1mm}\n");
  fprintf(fo,"%%\\fbox{\\begin{picture}(%.2f,%.2f)\n",widecm,highcm);
  fprintf(fo,"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% \n");
  fprintf(fo,"\\fbox{\\begin{picture}(%.2f,%.2f)\n",wide,high);
  FileNameChange(filein,fileout,base,"eps");
#ifdef EPSFIG
  fprintf(fo,"\\epsfig{file=\\Figdir %s.eps,width=%.2fpt,height=%.2fpt}\n",
	  base,wide,high);
#else
  fprintf(fo,"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
  fprintf(fo,"%% If you want do not want to use epsfig ,comment the following line \n");
  fprintf(fo,"%% and uncomment the \\special line \n");
  fprintf(fo,"\\epsfig{file=\\Figdir %s.eps,width=%.2fpt,height=%.2fpt}\n",
	  base,wide,high);
  fprintf(fo,"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
  fprintf(fo,"%%\\special{psfile=\\Figdir %s.eps hscale=%.2f vscale=%.2f}\n",
	  base,hscale,vscale);
#endif
  fprintf(fo,"\\end{picture}}\n");
  fprintf(fo,"\\end{center}\n");
#ifndef DOC 
  fprintf(fo,"\\caption{\\label{#2}#1}\n");
  fprintf(fo,"\\end{figure}}\n");
#endif
  fclose(fo);
  return(0);
}

/*****************************************
 * xxx/xxx/xxx/ffff.zzz -->  ffff.syf 
 * and name = "ffff"
 ****************************************/

static void FileNameChange(char *filein, char *fileout, char *name, char *suf)
{
  char *p=filein,*p1;
  p1=strchr(p,'/');
  while ( p1 != 0 ) 
    {
      p=p1+1;
      p1 = strchr(p,'/');
    }
  p1 = strchr(p,'.');
  if ( p1 != 0 ) 
    {
      *p1 = '\0';
      sprintf(fileout,"%s.%s",filein,suf);
      sprintf(base,"%s",p);
      *p1 = '.';
    }
  else 
    {
      sprintf(fileout,"%s.%s",filein,suf);
      sprintf(base,"%s",p);
    }
  /* 
     sciprint("[%s]=>[%s]\n",filein,fileout); 
     sciprint("real base %s\n",base);
     */
}


/**************************************************
 * copies file to fileo performing some substitutions 
 **************************************************/

static int Sed(int flag, char *file, FILE *fileo, char *strin1, char *strout1, char *strin2, char *strout2, char *strin3, char *strout3)
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
	   if ( flag == 1 ) 
	     {
	       if ( strncmp(buff,"%!PS-Adobe-2.0 EPSF-2.0",
			    strlen("%!PS-Adobe-2.0 EPSF-2.0"))== 0)
		 {
		   fclose(fd);
		   return(1);
		 }
	     }
	   if ( strin1 != (char *) 0 && strncmp(buff,strin1,strlen(strin1))==0)
	     fprintf(fileo,"%s\n",strout1);
	   else
	     {
	       if (  strin2 != (char *) 0 && strncmp(buff,strin2,strlen(strin2))==0)
		 fprintf(fileo,"%s\n",strout2);
	       else 
		 {
		   if ( strin3 != (char *) 0 && strncmp(buff,strin3,strlen(strin3))==0)
		     fprintf(fileo,"%s\n",strout3);
		   else
		     fprintf(fileo,"%s",buff);
		 }
	     }
	 }
      fclose(fd);
    }
  else 
    {
      sciprint("file %s not found \n",file);
      return(2);
    }
  return(0);
}

