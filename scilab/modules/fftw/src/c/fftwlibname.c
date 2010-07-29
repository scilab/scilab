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
#include "stdlib.h"
#include "fftwlibname.h"
#include "MALLOC.h"
#include "os_strdup.h"
/*--------------------------------------------------------------------------*/ 
char *fftwlibname = NULL;
/*--------------------------------------------------------------------------*/ 
void setfftwlibname(char *libname)
{
	if (libname)
	{
		if (fftwlibname) {FREE(fftwlibname);fftwlibname = NULL;}
		fftwlibname = os_strdup(libname);
	}
}
/*--------------------------------------------------------------------------*/ 
char *getfftwlibname(void)
{
	char *name = NULL;

	if (fftwlibname)
	{
		name = os_strdup(fftwlibname);
	}

	return name;
}
/*--------------------------------------------------------------------------*/ 
