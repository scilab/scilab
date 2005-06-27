/* Copyright INRIA/ENPC */
#include <stdio.h>
#include <string.h>

#include "../graphics/Math.h"
#include "Os_specific.h"

#define MAX_ENV 256 

static char *SCI_a[] = {  "SCI/", "sci/", "$SCI", "SCI\\", "sci\\", (char *) 0 };
static char *HOME_a[] = {  "HOME/", "home/", "~/" , "HOME\\", "home\\", "~\\" ,"$HOME", (char *) 0};
static char *TMP_a[] = {  "TMPDIR/", "tmpdir/","TMPDIR\\", "tmpdir\\", "$TMPDIR", (char *) 0};
void GetenvB __PARAMS(( char *name,char *env, int len));
static int Cluni0 __PARAMS((char *env,char **alias,char* in_name,char *out_name, long int lin));

extern int UpdateEnvVar; /* see setenvc.c */
static int n=0;
/************************************************
 * expand  in_name to produce out_name 
 *       
 ************************************************/

int C2F(cluni0)(char *in_name, char *out_name, int *out_n, long int lin, long int lout)
{
  int  nc= MAX_ENV;
  static char SCI[MAX_ENV],HOME[MAX_ENV],TMP[MAX_ENV];
  static int k;
  
  if ( ( n==0 ) || (UpdateEnvVar == 1) )
  {
	  GetenvB("SCI",SCI,nc);
	  GetenvB("HOME",HOME,nc);
      GetenvB("TMPDIR",TMP,nc);
	  n=n+1;
	  UpdateEnvVar=0;
  }
  /* in_name[lin]='\0';*/
  if ( Cluni0(SCI,SCI_a,in_name,out_name,lin) == 0 )
    if ( Cluni0(HOME,HOME_a,in_name,out_name,lin) == 0 )
      if ( Cluni0(TMP,TMP_a,in_name,out_name,lin) == 0 ){
	strncpy(out_name,in_name,(size_t)lin);
	out_name[lin]='\0';
      }
  /*strncpy(out_name,in_name,lout); */
  *out_n = strlen(out_name);
#if defined(THINK_C)||defined(__MWERKS__)
  for (k=0 ; k < *out_n ;k++) if ( out_name[k]=='/') out_name[k]=':';
#else
#if defined(WIN32)
  for (k=0 ; k < *out_n ;k++) if ( out_name[k]=='/') out_name[k]='\\';
#else
  for (k=0 ; k < *out_n ;k++) if ( out_name[k]=='\\') out_name[k]='/';
#endif
#endif


  /** sciprint( "out [%s] [%d]\r\n",out_name,*out_n); **/
  return(0);
}

/************************************************
 * getenv + squash trailing white spaces 
 ************************************************/

void GetenvB(char *name, char *env, int len)
{
  int ierr,un=1;
  C2F(getenvc)(&ierr,name,env,&len,&un);
  if( ierr == 0) 
    {
      char *last = &env[len-1];
      while ( *last == ' ' ) { last = '\0' ; } last--;
    }
  else 
    {
      env[0] = '\0' ;
    }  
}

/************************************************
 * expand in_name to produce out_name 
 *     try to find alias[i] at the begining of in_name 
 *     and replaces it by env in out_name 
 *     out_name must be large enough to get the result 
 ************************************************/

static int Cluni0(char *env, char **alias, char *in_name, char *out_name, long int lin)
{
  int i=0;
  if ( env[0] == '\0' ) return(0);
  while ( alias[i] != (char *) 0) 
    {
      if ( strncmp(alias[i],in_name,strlen(alias[i])) == 0)
	{
	  strcpy(out_name,env);
	  strncat(out_name,in_name+strlen(alias[i])-1,
		  (size_t)(lin-strlen(alias[i]+1)));
	  /*sprintf(out_name,"%s/%s",env,in_name+strlen(alias[i]));*/
	  return(1);
	}
      i++;
    }
  return(0);
}

