/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <Windows.h>
#include "buildMainWindowTitle_Windows.h"
#include "MALLOC.h"
#include "version.h"
#include "WndThread.h"
/*--------------------------------------------------------------------------*/
char *buildMainWindowTitle_Windows(void)
{
	char *title = NULL;

	title = (char*)MALLOC(sizeof(char)*(strlen("%s (%d)")+strlen(SCI_VERSION_STRING)+10+1));
	if (title) wsprintf(title,"%s (%d)",SCI_VERSION_STRING,getCurrentScilabId());

	return title;
}
/*--------------------------------------------------------------------------*/
