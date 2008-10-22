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
#ifdef _MSC_VER
#include <Windows.h>
#endif
#include <stdio.h>
#include "version.h"
#include "scilabmode.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
void disp_scilab_version(void)
{
	if ( (getScilabMode() == SCILAB_NWNI) || (getScilabMode() == SCILAB_NW) || (getScilabMode() == SCILAB_API) )
	{
		printf(_("Scilab version \"%d.%d.%d.%d\"\n"),SCI_VERSION_MAJOR,SCI_VERSION_MINOR,SCI_VERSION_MAINTENANCE,SCI_VERSION_TIMESTAMP);
		printf("%s\n\n",SCI_VERSION_STRING);
	}
	else
	{
		#ifdef _MSC_VER
		{
			char msg[1024];
			wsprintf(msg,_("Scilab version \"%d.%d.%d.%d\"\n%s\n"),SCI_VERSION_MAJOR,SCI_VERSION_MINOR,SCI_VERSION_MAINTENANCE,SCI_VERSION_TIMESTAMP,SCI_VERSION_STRING);
			MessageBox(NULL,msg,_("Scilab Version Info."),MB_ICONINFORMATION);
		}
		#else
		printf(_("Scilab version \"%d.%d.%d.%d\"\n"),SCI_VERSION_MAJOR,SCI_VERSION_MINOR,SCI_VERSION_MAINTENANCE,SCI_VERSION_TIMESTAMP);
		printf("%s\n\n",SCI_VERSION_STRING);
		#endif
	}
}
/*--------------------------------------------------------------------------*/ 
