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
#include "configvariable.hxx"

extern "C"
{
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>

#include "core_math.h"
#include "dynamic_link.h"
#include "men_Sutils.h"
#include "addinter.h" 

#include "do_error_number.h"
#include "stack-c.h"
#include "MALLOC.h" /* MALLOC */
#include "sciprint.h"
#include "Funtab.h"
#include "sci_warning.h"
#include "GetenvB.h"
#include "localization.h"
#include "Scierror.h"
#ifdef _MSC_VER
#include "ExceptionMessage.h"
#endif
}
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
int AddInterfaceToScilab(wchar_t* _pwstDynamicLibraryName, wchar_t* _pwstModuleName, wchar_t** _pwstEntryPointName, int _iEntryPointSize)
{
	int iLibID = -1; /* Id of library */
	int iErr = 0;
	
	/** Try to unlink the interface if it was previously linked **/
    ConfigVariable::EntryPointStr* pEP = ConfigVariable::getEntryPoint(_pwstModuleName);
    if(pEP)
    {//entry point already linked, so remove it before add it
        ConfigVariable::removeDynamicLibrary(pEP->iLibIndex);
    }

	/* link then search  */ 
	/* Haven't been able to find the symbol. Try C symbol */
    iLibID =  scilabLink(iLibID, _pwstDynamicLibraryName, &_pwstModuleName, 1, FALSE, &iErr);
    if(iErr)
	{
    	/* Trying with the fortran symbol */
        iLibID =  scilabLink(iLibID, _pwstDynamicLibraryName, &_pwstModuleName, 1, TRUE, &iErr);
        if(iErr)
        {
            return iErr;
        }
    }

    pEP = ConfigVariable::getEntryPoint(_pwstModuleName);
    if(pEP == NULL)
    {//
        return -1;
    }

    for(int i = 0 ; i < _iEntryPointSize ; i++)
    {
        pEP->functionPtr(_pwstEntryPointName[i]);
    }
	return 0;
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
		SciError(imes);
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
		SciError(imes);
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
