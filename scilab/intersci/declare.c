#include <stdlib.h>
#include "intersci-n.h"

/* global variables */

extern int indent ; /* incremental counter for code indentation */
extern char target ; /* langage for generation */

/**********************************************************
  Function to add decaration during the first pass 
  and to print them during code generation pass 2
**********************************************************/


static struct Declare {
  int type;
  char *nameF,*nameC;
  char **decls ; /* declaration de logical */
  int  ndecls;
} Init[] = {
  { DEC_CHAR,"character","char",(char **) 0,0},
  { DEC_INT ,"integer","int",(char **) 0,0},
  { DEC_DOUBLE,"double precision","double",(char **) 0,0},
  { DEC_REAL,"real","float",(char **) 0,0},
  { DEC_LOGICAL,"logical","int",(char **) 0,0},
  { DEC_DATA,"data","static",(char **) 0,0},
  { DEC_UL,"double precision","unsigned long",(char **) 0,0},
  { DEC_IPTR,"double precision","int ",(char **) 0,0},
  { DEC_DPTR,"double precision","double",(char **) 0,0},
  { DEC_RPTR,"double precision","float",(char **) 0,0},
  { DEC_CPTR,"double precision","char",(char **) 0,0},
  { DEC_SPARSE,"double precision","SciSparse",(char **) 0,0},
  { DEC_SPARSEPTR,"double precision","SciSparse",(char **) 0,0},
  { DEC_INIT,"","",(char **) 0,0},
  { DEC_SMAT,"Unimplemented","char ",(char **) 0,0},
  { -1 ,"void","void",(char **) 0,0}
};

void InitDeclare() 
{ 
  int i = 0;
  while ( Init[i].type != -1) 
    {
      Init[i].decls = (char **) 0;
      Init[i].ndecls =0 ;
      i++;
    }
}

void ResetDeclare()
{
  int j = 0;
  while ( Init[j].type != -1) 
    {
      if ( Init[j].decls != (char **) 0) 
	{
	  int i;
	  for ( i = 0 ; i < Init[j].ndecls ; i++ ) 
	    free((char *) Init[j].decls[i]);
	  free (( char *) Init[j].decls );
	}
      Init[j].decls=(char **) 0;
      Init[j].ndecls=0;
      j++;
    }
}

int  CheckDeclare(type,declaration)
     int type;
     char *declaration;
{
  int j = 0;
  while ( Init[j].type != -1) 
    {
      if ( Init[j].type == type ) 
	{
	  int i;
	  for ( i = 0 ; i < Init[j].ndecls ; i++ ) 
	    {
	      if ( strcmp(declaration,Init[j].decls[i])==0) 
		return(1);
	    }
	  return(0);
	}
      j++;
    }
  return(0);
}

/***************************
 * AddDeclare1(type,format,arg1,...,argn) 
 ***************************/

#define DECLAREBUF 128

#ifdef __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif 

#ifdef __STDC__ 
void AddDeclare1(int type,char *format,...) 
#else 
     /*VARARGS0*/
     void AddDeclare1(va_alist) va_dcl
#endif
{
  char decbuf[DECLAREBUF];
  va_list ap;
#ifdef __STDC__
  va_start(ap,format);
#else 
  int type;
  char *format;
  va_start(ap);
  type = va_arg(ap, int );
  format = va_arg(ap, char *);
#endif 
  vsprintf(decbuf,format,ap);
  AddDeclare(type,decbuf);
  va_end(ap);
}

void AddDeclare(type,declaration) 
     int type;
     char *declaration;
{
  int j = 0;
  if ( declaration[0] == '&' ) return ;
  if ( CheckDeclare(type,declaration)== 1) return ;
  while ( Init[j].type != -1) 
    {
      if ( Init[j].type == type ) 
	{
	  if ( Init[j].decls != (char **) 0) 
	    {
	      (Init[j].ndecls)++;
	      Init[j].decls =  (char **) realloc((char *) Init[j].decls, (unsigned) (Init[j].ndecls ) *sizeof(char *));
	    }
	  else 
	    {
	      (Init[j].ndecls)++;
	      Init[j].decls = (char **) malloc ( (unsigned) (Init[j].ndecls ) *sizeof(char *));
	    }
	  if ( Init[j].decls == ( char **) 0) 
	    {
	      fprintf(stderr,"No more space\n");
	      exit(1);
	    }
	  Init[j].decls[Init[j].ndecls-1]=(char*) malloc((unsigned) (strlen(declaration)+1)*sizeof(char));
	  if ( 	  Init[j].decls[Init[j].ndecls-1] == ( char *) 0) 
	    {
	      fprintf(stderr,"No more space\n");
	      exit(1);
	    }
	  strcpy(   Init[j].decls[Init[j].ndecls-1], declaration );
	}
      j++;
    }
}


void WriteInitDeclarations(f) 
     FILE *f;
{
  int j = 0;
  int i;
  while ( Init[j].type != -1) 
    {
      if ( Init[j].type == DEC_INIT) 
	{
	  for (i= 0 ; i < Init[j].ndecls ; i++) 
	    {
	      Fprintf(f,indent,"%s",Init[j].decls[i]);
	      Fprintf(f,indent,";\n");
	    }
	}
      j++;
    }
}


void WriteDeclaration(f) 
     FILE *f;
{
  int j = 0;
  int i;
  while ( Init[j].type != -1) 
    {      
      if ( Init[j].type == DEC_INIT) 
	{}
      else if( Init[j].type == DEC_DATA ) 
	{
	  for (i= 0 ; i < Init[j].ndecls ; i++) 
	    {
	      Fprintf(f,indent,"%s ",Init[j].nameC);
	      Fprintf(f,indent,"%s",Init[j].decls[i]);
	      Fprintf(f,indent,";\n");
	    }
	}
      else 
	{
	  if ( Init[j].ndecls != 0) 
	    Fprintf(f,indent,"%s ",Init[j].nameC);
	  for (i= 0 ; i < Init[j].ndecls ; i++) 
	    {
	      if ( Init[j].type >= DEC_IPTR && target == 'C')
		{
		  /* pointers declaration */
		  Fprintf(f,indent,"*");
		}
	      else if ( Init[j].type == DEC_SMAT && target == 'C')
		{
		  Fprintf(f,indent,"**");
		}
	      Fprintf(f,indent,"%s",Init[j].decls[i]);
	      if ( i != Init[j].ndecls -1 ) Fprintf(f,indent,",");
	      else 
		{
		  Fprintf(f,indent,";\n");
		}
	    }
	}
      j++;
    }
  j=0;
  WriteInitDeclarations(f);
}

