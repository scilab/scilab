/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2006 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "machine.h"
#include "SetScilabEnvironment.h"
#include "SCIHOME.h"
#include "inisci-c.h" /* SetSci */
#include "localization.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
	extern void SciEnvForWindows(void);
#endif
/*--------------------------------------------------------------------------*/
/**
* Define SCI and some others Environments variables 
*/
void SetScilabEnvironment(void) 
{
#ifdef _MSC_VER
	SciEnvForWindows(); 
#else
	SetSci();
#endif
	if (!setSCIHOME())
	{
		fprintf(stderr,_("Error: Impossible to define %s environment variable.\n"),"SCIHOME");
	}
}
/*--------------------------------------------------------------------------*/
