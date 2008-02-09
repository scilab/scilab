/*-----------------------------------------------------------------------------------*/
#include <string.h> 
#include <stdio.h>
#include "core_math.h"
#include "dynamic_link.h"
#include "../../../string/includes/men_Sutils.h"
#include "addinter.h" 

#include "stack-def.h"
#include "MALLOC.h" /* MALLOC */
#include "sciprint.h"
#include "Funtab.h"
#include "warningmode.h"
#include "GetenvB.h"
#include "localization.h"
#ifdef _MSC_VER
#include "ExceptionMessage.h"
#endif


extern int C2F(error)  __PARAMS((integer *n));  

#include <stdlib.h>

#define MAXINTERF 50
#define INTERFSIZE 25 
/*-----------------------------------------------------------------------------------*/
extern int C2F(cvname)();
/*-----------------------------------------------------------------------------------*/
typedef struct 
{
	char name[INTERFSIZE]; /** name of interface **/
	void (*func)();        /** entrypoint for the interface **/
	int Nshared; /** id of the shared library **/
	BOOL ok;    /** flag set to TRUE if entrypoint can be used **/
} InterfaceElement;

InterfaceElement DynInterf[MAXINTERF];
/*-----------------------------------------------------------------------------------*/
static int LastInterf=0;
static void initializeInterfaces(void);
/*-----------------------------------------------------------------------------------*/
int AddInterfaceToScilab(char *filenamelib,char *spname,char **fcts,int sizefcts)
{
	int ierr = 0;
	int i = 0;
	int inum = 0;
	int k1 = 0;
	int id[nsiz],zero=0,trois=3,fptr = 0,fptr1 = 0,quatre=4;

	initializeLink();
	initializeInterfaces();

	/** Try to unlink the interface if it was previously linked **/

	for ( i = 0 ; i < LastInterf ; i++) 
	{
		if (strcmp(spname,DynInterf[i].name) == 0) 
		{
			unlinksharedlib(&DynInterf[i].Nshared);
			break;
		}
	}

	/** Try to find a free position in the interface table : inum **/
	inum=-1;
	for ( i = 0 ; i < LastInterf ; i++) if ( DynInterf[i].ok == 0 ) inum= i;

	inum = ( inum == -1 ) ? LastInterf : inum ;

	/** Linking Files and add entry point name iname */

	if ( inum >=  MAXINTERF ) 
	{
		return -1;
	}
	else
	{
		int IdLib = -1;
		int idinput = -1;
		int ierr1 = 0;
		int one = 1;
		char **subname = NULL;

		subname = (char **)MALLOC(sizeof (char*));
		subname[0]= spname;

		/* link then search  */ 
		IdLib =scilabLink(idinput,filenamelib,subname,one,TRUE,&ierr1);
		subname[0]= NULL;
		if (subname) { FREE(subname);subname = NULL;}

		if ( IdLib < 0 ) return IdLib;

		/** store the linked function in the interface function table DynInterf **/
		DynInterf[inum].Nshared = IdLib;

		if ( SearchInDynLinks(spname,&DynInterf[inum].func) < 0 ) 
		{
			/* Maximum number of dynamic interfaces */
			return -6;
		}
		else
		{
			strncpy(DynInterf[inum].name,spname,INTERFSIZE);
			DynInterf[inum].ok = TRUE;
		}
		if ( inum == LastInterf ) LastInterf++;

		k1 = inum+1;
		for (i = 0;i < sizefcts; i++)
		{
			C2F(cvname)(id,fcts[i],&zero,strlen(fcts[i]));
			fptr1 = fptr = (DynInterfStart+k1)*100 +(i+1);
			C2F(funtab)(id,&fptr1,&quatre,"NULL_NAME",0); /* clear previous def set fptr1 to 0*/
			C2F(funtab)(id,&fptr,&trois,"NULL_NAME",0);  /* reinstall */
		}
	}
	return ierr;
}
/*-----------------------------------------------------------------------------------*/
static void initializeInterfaces(void)
{
	static int first_entry = 0;

	if ( first_entry == 0) 
	{
		int i = 0;
		for ( i= 0 ; i < MAXINTERF ; i++) 
		{
			strcpy(DynInterf[i].name,"");
			DynInterf[i].func = NULL;

			DynInterf[i].Nshared = -1;
			DynInterf[i].ok = FALSE;
		}

		first_entry++;
	}
}
/*-----------------------------------------------------------------------------------*/
/*********************************
* used in unlinksharedlib(i) 
*********************************/
void RemoveInterf(int id)
{
	int i = 0;
	for ( i = 0 ; i < LastInterf ; i++ ) 
	{
		if ( DynInterf[i].Nshared == id ) 
		{
			DynInterf[i].ok = FALSE;
			break;
		}
	}
}
/*-----------------------------------------------------------------------------------*/
/************************************************
* Used when one want to call a function added 
* with addinterf the dynamic interface number 
* is given by k1=(*k/100)-1
************************************************/
void C2F(userlk)(integer *k)
{
	int k1 = *k - (DynInterfStart+1) ;
	int imes = 9999;
	if ( k1 >= LastInterf || k1 < 0 ) 
	{
		if (getWarningMode()) sciprint(_(" results may be inaccurate. rcond = %s\n"),k1);
		C2F(error)(&imes);
		return;
	}
	if ( DynInterf[k1].ok == 1 ) 
	{
#ifdef _MSC_VER
#ifndef _DEBUG
		_try
		{
			(*DynInterf[k1].func)();
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
			ExceptionMessage(GetExceptionCode(),DynInterf[k1].name);
		}
#else
		(*DynInterf[k1].func)();
#endif
#else
		(*DynInterf[k1].func)();
#endif
	}
	else 
	{
		if (getWarningMode()) sciprint(_("Interface %s not linked.\n"),DynInterf[k1].name);
		C2F(error)(&imes);
		return;
	}
}
/*-----------------------------------------------------------------------------------*/


