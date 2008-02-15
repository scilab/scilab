/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include "mopen.h"
#include "Status2Mode.h"
#include "filesmanagement.h"
#include "getfiledesc.h"
#include "addfile.h"
#include "sciprint.h"
#include "machine.h"
#include "localization.h"
#include "warningmode.h"
/*--------------------------------------------------------------------------*/
extern int swap;
/*--------------------------------------------------------------------------*/
void C2F(mopen)(int *fd, char *file, char *status, int *f_swap, double *res, int *error)
{   
	int	littlendian = 1,type = 2,ierr,mode;
	char	*endptr;
	FILE * fa;

	if ( getWarningMode() && IsAlreadyOpenedInScilab(file) )
	{
		sciprint(_("Warning: file '%s' already opened in Scilab.\n"),file);
	}

	swap =0;
	*error=0;
	endptr = (char *) &littlendian;
	if ( (!*endptr) )
	{
		if( *f_swap == 1 ) swap = 1;
		else swap =0;
	}

	C2F(getfiledesc)(fd);

	if ( *fd == -1 )
	{
		*error=1; /* Too many opened files */
		return;
	}
	fa=fopen(file,status);
	if (! fa )
	{     
		*error=2; /* Could not open the file*/
		return;
	}
	mode=Status2Mode(status);
	C2F(addfile)(fd,fa,&swap,&type,&mode,file,&ierr);
	if (ierr)
	{
		*error=3; /* Not enough memory;*/
		return;
	}
	SetCurrentFileId(*fd);
	*res = (double)ferror(fa);
}
/*--------------------------------------------------------------------------*/

