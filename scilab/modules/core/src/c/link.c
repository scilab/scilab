/* Copyright INRIA/ENPC */

/********************************************* 
 *   Scilab link functions    
 ******************************************/ 
#ifdef sgi
#define _KMEMUSER 1
#endif

#include <string.h> 
#include <stdio.h>

#include "core_math.h"
#include "MALLOC.h" /* MALLOC */
#include "../../../string/includes/men_Sutils.h"
#include "link.h"
#include "sciprint.h"

#include <stdlib.h>

#include "machine.h"
#include "stack-c.h"
#include "warningmode.h"
#include "message_scilab.h"

static void Underscores __PARAMS((int isfor,char *ename,char *ename1));
static int SearchFandS  __PARAMS( ( char *,int ));
static void ShowDynLinks(void);
int LinkStatus __PARAMS((void)) ;
int C2F(scilinknorhs)(void);


/*********************************************
 * Structure to keep the entry points 
 *********************************************/

#define OK 1
#define FAIL 0


#define ENTRYMAX 5000
#define MAXNAME  256 

typedef void (*function) ();

typedef char Name[MAXNAME];   /* could be changed to dynamic structure */

typedef struct { 
  function epoint;            /* the entry point */ 
  Name     name;              /* entry point name */
  int      Nshared;           /* number of the shared file */
} Epoints;

typedef struct {
  int ok;
  char tmp_file[PATH_MAX];
  unsigned long  shl;
} Hd;

static Hd  hd[ENTRYMAX]; /* shared libs handler */
static int Nshared  = 0   ;
static Epoints EP[ENTRYMAX];  /* entryPoints */
static int NEpoints = 0   ;        /* Number of Linked names */

/*-----------------------------------------------------------------------------------*/
int C2F(scilinknorhs)()
{
	int i=0;
	static int l1,n1,m1;
	char **ReturnArrayString=NULL;
	int j=0;
	m1=NEpoints;
	n1=1;

	if (NEpoints)
	{
		ReturnArrayString = (char **) MALLOC(NEpoints*sizeof(char **));

		for ( i = NEpoints-1 ; i >=0 ; i--) 
		{
			char *EntryName=(char *)MALLOC(strlen(EP[i].name)*sizeof(char));
			if ( hd[i].ok == OK) 
			{
				strcpy(EntryName,EP[i].name);
				ReturnArrayString[j]=EntryName;
				j++;
			}
		}

		CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &n1, &m1, ReturnArrayString);

		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();

		for (i=0;i<NEpoints;i++)
		{
			if (ReturnArrayString[i])
			{
				FREE(ReturnArrayString[i]);
				ReturnArrayString[i]=NULL;
			}
		}
		FREE(ReturnArrayString);
	}
	else
	{
		m1=0;
		n1=0;
		l1=0;
		CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
	}


	return 0;
}
/*-----------------------------------------------------------------------------------*/

/** for debug info **/
/** #define DEBUG  **/

/************************************************
 * Link facilities 
 * Interface to work with Scilab 
 * descla,ptrdescla,nvla : string matrix if *iflag == 0 
 * desc,ptrdesc,nv       : string matrix ,
 * string  strf          : "f" OR "c" 
 * ilib                  : integer (out/in value )
 * iflag                 : 0 a string matrix is used 1 ilib is used
 * rhs                   : number of rhs arguments in link(...)
 ************************************************/

void C2F(iscilink)(int *descla, int *ptrdescla, int *nvla, int *desc, int *ptrdesc, int *nv, char *strf, int *ilib, int *iflag, int *rhs)
                                     /* files */
                                     /* entry points */
                
                                                 
{
  int ierr,i;
  char **files=NULL,**en_names=NULL;
  ierr=0;
  en_names=NULL; /* Initialisation variable pour Windows */

  if ( *iflag == 0) 
    {
      ScilabMStr2CM(descla,nvla,ptrdescla,&files,&ierr);
      if ( ierr == 1) return;
    }
  if ( *rhs >= 2) 
    {
      ScilabMStr2CM(desc,nv,ptrdesc,&en_names,&ierr);
      if ( ierr == 1) return;
    }

  SciLinkInit();
  
  if ( *iflag== 0 &&  strncmp(files[0],"show",4)==0) 
    {
      ShowDynLinks();
      *ilib = LinkStatus();  /* return value for Scilab */
      return;
    }

  /** calling the linker **/
  
  SciLink(*iflag,rhs,ilib,files,en_names,strf);

  if ( *rhs >= 2) 
    {
      for (i=0;i< *nv;i++) FREE(en_names[i]); FREE(en_names);
    }

  if ( *iflag == 0) 
    {
      for (i=0;i< *nvla;i++) FREE(files[i]); FREE(files);
    }
  if (*ilib >= 0) 
  {
	  if (getWarningMode()) message_scilab(_("Link done."));
  }
}


#if defined(netbsd) || defined(freebsd) || defined(sun) || defined(__alpha) || defined(sgi) || (!defined(hppa_old) && defined(hppa))  || defined(__APPLE__)
#include "link_SYSV.c"
#else
/** no more used on sun */
#if (defined(sun_old) ||  (defined(mips) && !defined(netbsd)) || defined(_IBMR2) || defined(hppa_old)) && !defined(linux)
#ifdef SUNOSDLD 
#include "link_linux.c"
#else 
#include "link_std.c"
#endif /* end of SUNOSDLD */
#else
#if defined(linux)
#ifdef __ELF__
#include "link_SYSV.c"
#else
#include "link_linux.c"
#endif /* end of __ELF__ */
#else
#if defined(_MSC_VER)
#include "link_W95.c"
#else
/**
  C2F(dynload)() {cerro("Dynamic link not implemented");}
  C2F(dyncall)() {cerro("Dynamic link not implemented");}
  **/
int LinkStatus()
{
  return(0);
}

void SciLink(iflag,rhs,ilib,files,en_names,strf)
     int iflag,*ilib,*rhs;
     char *files[],*en_names[],*strf;
{
  if (getWarningMode()) message_scilab(_("Sorry : Dynamic linking is not implemented."));
}


void C2F(isciulink)(i) 
     integer *i;
{
  if (getWarningMode()) message_scilab(_("Sorry : Unlinking is not implemented."));
}

#endif
#endif
#endif 
#endif 

#ifdef WLU
#ifndef DLDLINK
#ifndef _MSC_VER
#ifndef __APPLE_CC__
#define WLU1 /* dld will add the leading _ itself, win32 too, dlcompat too */
#endif
#endif 
#endif
#endif 

/********************************************
 * Underscores : deals with the trailing _ 
 * in entry names 
 ********************************************/

static void Underscores(int isfor, char *ename, char *ename1)
{
#ifdef WLU1
  *ename1='_'; ename1++;
#endif
  strcpy(ename1,ename);
#ifdef WTU
  if (isfor==1) strcat(ename1,"_");
#endif
  return;
}

/**************************************
 * Initialize tables 
 *************************************/

void SciLinkInit(void)
{
  static int first_entry = 0;
  int i;
  if ( first_entry == 0)
    {
      
      for ( i = 0 ; i < ENTRYMAX ; i++) 
	{
	  hd[i].ok= FAIL;
	  EP[i].Nshared = -1;
	}
      first_entry++;
    }
}

/**************************************
 * if *irep == -1 
 *    checks if buf is a loaded
 *    entry point 
 *    the result is -1 if false 
 *               or the number in the function table 
 * 
 *    
 * if *irep != -1 : 
 *    checks if buf is a loaded
 *    entry point from shared lib *irep
 *    the result is -1 if false 
 *               or the number in the function table 
 * 
 *************************************/

void C2F(iislink)(char *buf, integer *irep)
{
  void (*loc)();
  if ( *irep != -1 ) 
    *irep=SearchFandS(buf,*irep);
  else
    *irep=SearchInDynLinks(buf,&loc);
}


/**************************************
 * returns the ii functions 
 *************************************/

void GetDynFunc(int ii, void (**realop) (/* ??? */))
{
  if ( EP[ii].Nshared != -1 ) 
    *realop = EP[ii].epoint;
  else
    *realop = (function) 0;
}

/**************************************
 * Search a function in the table 
 * Search from end to top 
 *************************************/

int SearchInDynLinks(char *op, void (**realop) (/* ??? */))
{
  int i=0;
  for ( i = NEpoints-1 ; i >=0 ; i--) 
    {
      if ( strcmp(op,EP[i].name) == 0) 
	{
           *realop = EP[i].epoint;
	   return(EP[i].Nshared );
	 }
    }
  return(-1);
}


/**************************************
 * Search a (function,libid) in the table 
 * Search from end to top 
 *************************************/

static int SearchFandS(char *op, int ilib)
{
  int i=0;
  for ( i = NEpoints-1 ; i >=0 ; i--) 
    {
      if ( strcmp(op,EP[i].name) == 0 && EP[i].Nshared == ilib)
	{
	   return(i);
	 }
    }
  return(-1);
}

/**************************************
 * Show the linked files 
 *************************************/

static void ShowDynLinks(void)
{
  int i=0,count=0;
  if (getWarningMode()) message_scilab(_("Number of entry points %d."),NEpoints);
  if (getWarningMode()) message_scilab(_("Shared libs : ["));
  for ( i = 0 ; i < Nshared ; i++) 
    if ( hd[i].ok == OK) { if (getWarningMode())sciprint(_("%d "),i);count++;}
  if (getWarningMode()) message_scilab(_("] : %d libs."),count);
  for ( i = NEpoints-1 ; i >=0 ; i--) 
    {
      if (getWarningMode()) message_scilab(_("Entry point %s in shared lib %d."),EP[i].name,EP[i].Nshared);
    }
}

void ulinkall(void)
{
	int i=0;
	
	for ( i = 0 ; i < Nshared ; i++)
	{
		if ( hd[i].ok == OK) Sci_Delsym(i);
	}
    
}
