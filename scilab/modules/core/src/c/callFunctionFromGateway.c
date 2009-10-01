/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "callFunctionFromGateway.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
void callFunctionFromGateway(gw_generic_table *Tab,int sizeTab)
{
	if ( (Fin > sizeTab) || (Fin < 1) )
	{
		Scierror(999,_("Error: Not a valid primitive ID %d.\n"),Fin);
	}
	else
	{
#ifdef _MSC_VER
#ifndef _DEBUG
	_try
	{
		if (*(Tab[Fin-1].f) != NULL) (*(Tab[Fin-1].f)) ((char*)Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
	}
	_except (EXCEPTION_EXECUTE_HANDLER)
	{	
		ExceptionMessage(GetExceptionCode(),(char*)Tab[Fin-1].name);
	}
#else
	if (*(Tab[Fin-1].f) != NULL) (*(Tab[Fin-1].f)) ((char*)Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
#endif
#else
	if (*(Tab[Fin-1].f) != NULL) (*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
#endif
	}
}
/*--------------------------------------------------------------------------*/
