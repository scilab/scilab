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
#define Min(x,y)  (((x)<(y))?(x):(y))

static int Sed (int,char *,FILE *,char *,char *,char *,char *,char *,char *);
int ScilabPsToEps (char orientation,char *filein,char *fileout);
static  void dos2win32 (char *filename,char *filename1);
static void ConvertName (char *filein,char *fileout);
static void get_dims(char *file, int *w, int *h);

/**************************************************
 * SEpsf  Usage : BEpsf [-orientation] filename.ps 
 * used to produce an EPSF file from a scilab postscript file 
 * without Postscript preamble
 **************************************************/

char * UsageStr[]={
  "\tfilename.ps : Postscript file produced by Scilab\n",
  "\torientation : p[ortrait] or l[andscape]  \n",
  "fin"};

#define PATH_MAX 1024

static char file1[PATH_MAX],file2[PATH_MAX];

int main(int argc,char *argv[])
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

static  void dos2win32(char *filename,char *filename1)
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

static char entete[PATH_MAX];

int ScilabPsToEps(char orientation,char *filein,char *fileout)
{
  /* A4 paper in mm */
  double ccm = 28.346457;
  double wp = ccm*210;
  double hp = ccm*297;
  double ws,hs,sc,marg= ccm*5; /* margin 5mm */

  int flag = 0,rep, w=0,h=0;
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
  /* see if the postscript file was generated with non standard sizes */

  get_dims(filein,&w,&h);

  if ( w == 0 || h == 0 ) 
    {
      w= 6000; h = 4240; /* default dimensions */
    }
    
  /* The Postscript dimension will (w,h) proportions */
  fprintf(fo,"%%!PS-Adobe-2.0 EPSF-2.0\n");
  if ( orientation == 'p' ) 
    {
      ws = (wp-2*marg)/((double) w);
      hs = (hp-2*marg)/((double) h);
      sc = Min(ws,hs);
      ws = w*sc;
      hs = h*sc; 
      fprintf(fo,"%%%%BoundingBox: %d %d %d %d \n", 
	      (int) ((wp - ws)/(2*10.0)),
	      (int) ((hp - hs)/(2*10.0)),
	      (int) (ws/10.0 + (wp - ws)/(2*10.0)),
	      (int) (hs/10.0 + (hp - hs)/(2*10.0))
	      );
    }
  else 
    {
      ws = (hp-2*marg)/((double) w);
      hs = (wp-2*marg)/((double) h);
      sc = Min(ws,hs);
      ws = w*sc;
      hs = h*sc; 
      fprintf(fo,"%%%%BoundingBox: %d %d %d %d \n", 
	      (int) ((wp - hs)/(2*10.0)),
	      (int) ((hp - ws)/(2*10.0)),
	      (int) (hs/10.0 + (wp - hs)/(2*10.0)) ,
	      (int) (ws/10.0 + (hp - ws)/(2*10.0))
	      );
      
    }
  
  Sed(0,entete,fo,"%!PS-ADOBE","%%",(char*) 0,(char *)0,(char*) 0,(char *)0);
  
  if ( orientation == 'p' )
    {
      char cc[512];
      sprintf(cc,"[%f 20 div 0 0 %f 20 div neg %d 10 div %d 10 div] concat",
	      sc,sc,(int) ((wp - ws)/(2)), (int) ((hp - hs)/(2) + hs));
      rep=Sed(1,filein,fo,"[0.5 10 div 0 0 0.5 10 div neg",
	      cc,
	      (char*) 0,(char *)0,(char*) 0,(char *)0);
    }
  else 
    {
      char cc[512];
      sprintf(cc,"90 rotate 0 neg %d neg 10 div translate\n[%f 20 div 0 0 %f 20 div neg %d 10 div %d 10 div] concat",
	      h + (int) ((wp - hs)/(2.0)) ,  sc,sc,(int) ((hp - ws)/2), h ); 
      rep=Sed(1,filein,fo,"[0.5 10 div 0 0 0.5 10 div neg",
	      cc,
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


static void ConvertName(char *filein,char *fileout)
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



/*----------------------------------------------------
 * copies file to fileo performing some substitutions 
 *----------------------------------------------------*/

int Sed(int flag, char *file, FILE *fileo, char *strin1, char *strout1,
	char *strin2, char *strout2, char *strin3, char *strout3)
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
      sciprint("file %s not found \r\n",file);
      return(2);
    }
  return(0);
}


/*----------------------------------------------------
 * get w and h in the postscript file 
 *----------------------------------------------------*/

static void get_dims(char *file, int *w, int *h)
{
  FILE *fd;
  static char scipos_w[] = "%scipos_w=";
  static char scipos_h[] = "%scipos_h=";
  static char stop_s[] = "% Dessin en bas a gauche";

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
    { 
      int stop=0;
      while ( stop != 1)
	{ 
	  read_one_line (&buff,&stop,fd,&buflen); 
	  if ( strncmp(buff,scipos_w,strlen(scipos_w))==0)
	    {
	      sscanf(buff+strlen(scipos_w),"%d",w);
	    }
	  if ( strncmp(buff,scipos_h,strlen(scipos_h))==0)
	    {
	      sscanf(buff+strlen(scipos_h),"%d",h);
	    }
	  if ( strncmp(buff,stop_s,strlen(stop_s))==0 )
	    break;
	}
      fclose(fd);
    }
}

