/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifdef _MSC_VER
	#include <Windows.h> /* GetEnvironmentVariable */
#else
	#include <stdlib.h> /* getenv */
#endif
#include <string.h> /* strlen */
#include "getenvc.h"
#include "localization.h"
#include "sciprint.h"
/*--------------------------------------------------------------------------*/
void C2F(getenvc)(int *ierr,char *var,char *buf,int *buflen,int *iflag)
{
	#ifdef _MSC_VER
	if (GetEnvironmentVariable(var,buf,(DWORD)*buflen) == 0)
	{
		if ( *iflag == 1 ) sciprint(_("Undefined environment variable %s.\n"),var);
		*ierr=1;
	}
	else
	{
		*buflen = (int)strlen(buf);
		*ierr=0;
	}
	#else
	char *local;
	if ( (local=getenv(var)) == 0)
	{
		if ( *iflag == 1 ) sciprint(_("Undefined environment variable %s.\n"),var);
		*ierr=1;
	}
	else
	{
		strncpy(buf,local,*buflen);
		*buflen = strlen(buf);
		*ierr=0;
	}
	#endif
}
/*--------------------------------------------------------------------------*/

