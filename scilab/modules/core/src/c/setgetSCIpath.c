/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "machine.h"
#include "setgetSCIpath.h"
#include "MALLOC.h"
#include "string.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/ 
static char SCIPATH[PATH_MAX];
/*--------------------------------------------------------------------------*/ 
void setSCIpath(char *path)
{
	if (path) strcpy(SCIPATH,path);
}
/*--------------------------------------------------------------------------*/ 
char *getSCIpath(void)
{
	return strdup(SCIPATH);
}
/*--------------------------------------------------------------------------*/ 
