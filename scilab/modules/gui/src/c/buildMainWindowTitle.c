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
#include "buildMainWindowTitle.h"
#ifdef _MSC_VER
#include "../../../windows_tools/src/c/scilab_windows/buildMainWindowTitle_Windows.h"
#endif
#include "version.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
char *buildMainWindowTitle(void)
{
	char *retTitle = NULL;

	#ifndef _MSC_VER
	retTitle = strdup(SCI_VERSION_STRING);
	#else
	retTitle = buildMainWindowTitle_Windows();
	#endif
	return retTitle;
}
/*--------------------------------------------------------------------------*/
