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

static int Sed __PARAMS((int,char *,FILE *,char *,char *,char *,char *,char *,char *));
int ScilabPsToEps __PARAMS((char orientation,char *filein,char *fileout));
static  void dos2win32 __PARAMS((char *filename,char *filename1));
static void readOneLine __PARAMS((char *buff,int *stop,FILE *fd));
static void ConvertName __PARAMS((char *filein,char *fileout));

/**************************************************
 * SEpsf  Usage : BEpsf [-orientation] filename.ps 
 * used to produce an EPSF file from a scilab postscript file 
 * without Postscript preamble
 **************************************************/

char * UsageStr[]={
  "\tfilename.ps : Postscript file produced by Scilab\n",
  "\torientation : p[ortrait] or l[andscape]  \n",
  "fin"};

static char file1[256],file2[256];

int main(argc, argv)
     int argc;
     char *argv[];
{
  char orientation='p';
  if (argc >= 4 || argc <= 1) 
    { int i=0;
      fprintf(stderr,"Usage  : %s [-orientation] filename.ps \n",argv[0]);
      while (strcmp(UsageStr[i],"fin")!=0)
	  fprintf(stderr,"%s",UsageStr[i]),i++;
      return(1);
    }
  switch ( argc ) 
    {
    case 2: 
      if ( strncmp( argv[1] ,"-p",2) == 0 || strncmp( argv[1] ,"-l",2) == 0) 
	{
	  fprintf(stderr,"filename argument missing\n");
	  exit(1);
	}
      orientation = 'p';
      strcpy(file1,argv[1]);
      break;
    case 3:
      strcpy(file1,argv[2]);
      if ( strncmp( argv[1] ,"-p",2) == 0) 
	orientation = 'p';
      else if ( strncmp( argv[1] ,"-l",2) == 0) 
	orientation = 'l';
      else 
       fprintf(stderr,"Invalid first argument %s\n",argv[1]);
      break;
    }
  /** converts file1 format into file2 **/
  dos2win32(file1,file2);
  /** file2 : input file , file1 : output file **/
  ScilabPsToEps(orientation,file2,file1);
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

static  void dos2win32(filename,filename1)
char *filename, *filename1;
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

/** file wmprint.c in wsci ( should be gathered in a unique 
  lib ) **/

/**************************************************
 * Converts a scilab Eps file to an Epsf file 
 * by adding a preamble 
 **************************************************/

static char entete[256];

int ScilabPsToEps(orientation,filein,fileout)
     char orientation,*filein, *fileout;
{
  int flag = 0,rep;
  FILE *fo;
  char *env;
  env = getenv("SCI");
  if (env == NULL) 
    {
      sciprint("Environment variable SCI must be defined\r\n");
      return(1);
    }
  sprintf(entete,"%s/imp/NperiPos.ps",env);

  ConvertName(filein,fileout);
  if ( strcmp(fileout,filein) == 0 ) 
    {
      flag = 1;
      strcat(fileout,".temp");
    }
  
  if ( (fo = fopen(fileout,"w"))== 0 ) 
    {
      sciprint(" Can't open file %s\r\n",fileout);
      exit(1);
    }

  fprintf(fo,"%%!PS-Adobe-2.0 EPSF-2.0\n");
  if ( orientation == 'p' ) 
    fprintf(fo,"%%%%BoundingBox: 0 200 600 624\n");
  else 
    fprintf(fo,"%%%%BoundingBox: 0 0 600 780\n");

  Sed(0,entete,fo,"%!PS-ADOBE","%%",(char*) 0,(char *)0,(char*) 0,(char *)0);

  if ( orientation == 'p' )
    rep=Sed(1,filein,fo,"[0.5 10 div 0 0 0.5 10 div neg  0 2120 10 div] concat",
	"[0.5 5 div 0 0 0.5 5 div neg  0 3120 5 div] concat",
	(char*) 0,(char *)0,(char*) 0,(char *)0);
  else {
      fprintf(fo,"1.3 1.3  scale \n");
      rep=Sed(1,filein,fo,"[0.5 10 div 0 0 0.5 10 div neg  0 2120 10 div] concat",
	"90 rotate 10 640 neg translate [0.5 5 div 0 0 0.5 5 div neg  0 3120 5 div] concat",
	(char*) 0,(char *)0,(char*) 0,(char *)0);
  }
  fclose(fo);

  if ( rep >= 1 ) 
    {
      if ( rep == 1) 
	sciprint("input file doesn't need to be changed to epsf \r\n");
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
  return(0);
}


static void ConvertName(filein,fileout)
     char *filein,*fileout;
{
  char *p=filein,*p1;
  p1=strchr(p,'/');
  while ( p1 != 0 ) 
    {
      p=p1+1;
      p1 = strchr(p,'/');
    }
  p = strchr(p,'.');
  if ( p != 0 ) 
    {
      *p = '\0';
      sprintf(fileout,"%s.eps",filein);
      *p = '.';
    }
  else 
      sprintf(fileout,"%s.eps",filein);
  /** sciprint("[%s]=>[%s]\r\n",filein,fileout); **/
}


/**************************************************
 * copies file to fileo performing some substitutions 
 **************************************************/

static int Sed(flag,file,fileo,strin1,strout1,strin2,strout2,strin3,strout3)
     char file[],strin1[],strout1[],strout3[];
     char strin2[],strout2[],strin3[];
     FILE *fileo;
     int flag;
{
  FILE *fd;
  fd=fopen(file,"r");
  if (fd != 0)
    { int stop=0;
      while ( stop != 1)
	{  char buff[512];
	   readOneLine (buff,&stop,fd); 
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
      sciprint("file %s not found \r\n",file);
      return(2);
    }
  return(0);
}

/*-----------------------------------------------
  lit une ligne dans fd et la stocke dans buff
---------------------------------------------------*/

static void readOneLine(buff,stop,fd)
     char buff[];
     int *stop;
     FILE *fd;
{ int i ,c ;
  for ( i = 0 ;  (c =getc(fd)) !=  '\n' && c != EOF ; i++) buff[i]= c ;
  buff[i]='\n';buff[i+1]='\0';
  if ( c == EOF) {*stop = 1;}
}
