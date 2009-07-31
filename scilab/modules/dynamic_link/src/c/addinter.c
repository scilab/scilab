/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA/ENPC
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*-----------------------------------------------------------------------------------*/
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>

#include "core_math.h"
#include "dynamic_link.h"
#include "../../../string/includes/men_Sutils.h"
#include "addinter.h" 

#include "do_error_number.h"
#include "stack-c.h"
#include "MALLOC.h" /* MALLOC */
#include "sciprint.h"
#include "Funtab.h"
#include "warningmode.h"
#include "GetenvB.h"
#include "localization.h"
#include "Scierror.h"
#ifdef _MSC_VER
#include "ExceptionMessage.h"
#endif

/* size of interface name */
/* scilab limitation to nlgh characters (24)*/
#define INTERFSIZE nlgh + 1 
/*-----------------------------------------------------------------------------------*/
typedef struct 
{
	char name[INTERFSIZE]; /** name of interface **/
	void (*func)();        /** entrypoint for the interface **/
	int Nshared; /** id of the shared library **/
	BOOL ok;    /** flag set to TRUE if entrypoint can be used **/
} InterfaceElement;
/*-----------------------------------------------------------------------------------*/
InterfaceElement *DynInterf = NULL;
static int MaxInterfaces = MAXDYNINTERF;
/*-----------------------------------------------------------------------------------*/
static int LastInterf = 0;
static void initializeInterfaces(void);
static BOOL reallocDynInterf(void);
/*-----------------------------------------------------------------------------------*/
int AddInterfaceToScilab(char *filenamelib,char *spname,char **fcts,int sizefcts)
{
	int IdLib = -1; /* Id of library */
	int idinput = -1; /* Id of a function */
	int ierr1 = 0;
	int one = 1;
	char **subname = NULL;
	int ierr = 0;
	int i = 0;
	int inum = 0;
	int k1 = 0;
	
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
	for ( i = 0 ; i < LastInterf ; i++) 
	{
		if ( DynInterf[i].ok == 0 ) 
		{
			inum= i;
		}
	}

	inum = ( inum == -1 ) ? LastInterf : inum ;

	/** Linking Files and add entry point name iname */

	if ( inum >=  MaxInterfaces ) 
	{
		/* Try to resize DynInterf */
		if ( ( !reallocDynInterf() ) || ( inum >=  MaxInterfaces ) ) return -1;
	}

	subname = (char **)MALLOC(sizeof (char*));
	subname[0]= spname;

	/* link then search  */ 
	/* Trying with the fortran symbol */
	IdLib =  scilabLink(idinput,filenamelib,subname,one,TRUE,&ierr1);
	if (ierr1!=0)
	{
		/* Haven't been able to find the symbol. Try C symbol */
		IdLib =  scilabLink(idinput,filenamelib,subname,one,FALSE,&ierr1);
	}

	subname[0] = NULL;
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
		int id[nsiz],zero=0,three=3,fptr = 0,fptr1 = 0,four=4;

		/* find a previous functions with same name */
		C2F(cvname)(id,fcts[i],&zero,(unsigned long)strlen(fcts[i]));
		fptr1 = fptr = (DynInterfStart+k1)*1000 +(i+1);
		/* clear previous def set fptr1 to 0*/
		C2F(funtab)(id,&fptr1,&four,"NULL_NAME",0); 
		/* reinstall */
		C2F(funtab)(id,&fptr,&three,fcts[i],(unsigned long)strlen(fcts[i])); 
	}

	return ierr;
}
/*-----------------------------------------------------------------------------------*/
static void initializeInterfaces(void)
{
	static int first_entry_interfaces = 0;

	if ( first_entry_interfaces == 0) 
	{
		if (DynInterf == NULL)
		{
			DynInterf = (InterfaceElement*)MALLOC(sizeof(InterfaceElement)*MaxInterfaces);
			if (DynInterf)
			{
				int i = 0;
				for ( i= 0 ; i < MaxInterfaces ; i++) 
				{
					strcpy(DynInterf[i].name,"");
					DynInterf[i].func = NULL;

					DynInterf[i].Nshared = -1;
					DynInterf[i].ok = FALSE;
				}
			}
		}
		first_entry_interfaces++;
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
* is given by *k - (DynInterfStart+1)
************************************************/
void C2F(userlk)(int *k)
{
	int k1 = *k - (DynInterfStart+1) ;

	int imes = 9999;
	if ( k1 >= LastInterf || k1 < 0 ) 
	{
		if (getWarningMode()) 
		{
			Scierror(999,_("Error: Not a valid internal routine number %d.\n"), *k);
		}
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
static BOOL reallocDynInterf(void)
{
	/* increase table of interfaces by 2 */
	int newMaxInterfaces = MaxInterfaces * 2;

	if (newMaxInterfaces < ENTRYMAX)
	{
		if (DynInterf)
		{
			int i = 0;
			InterfaceElement *newDynInterf = NULL;

			newDynInterf = (InterfaceElement*)REALLOC(DynInterf,sizeof(InterfaceElement)*newMaxInterfaces);
			if (newDynInterf == NULL) return FALSE;

			DynInterf = newDynInterf;

			for ( i= MaxInterfaces ; i < newMaxInterfaces ; i++) 
			{
				strcpy(DynInterf[i].name,"");
				DynInterf[i].func = NULL;
				DynInterf[i].Nshared = -1;
				DynInterf[i].ok = FALSE;
			}
			MaxInterfaces = newMaxInterfaces;
			return TRUE;
		}
		else return FALSE;
	}
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/
