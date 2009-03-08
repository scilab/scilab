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
#include "mgetstr.h"
#include "filesmanagement.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
void C2F(mgetstr1) (int *fd, char *start, int *n, int *ierr)
{ 
	int count=0;
	FILE *fa=NULL;
	fa = GetFileOpenedInScilab(*fd);
	*ierr=0;
	if (fa)
	{ 
		count=(int)fread(start,sizeof(char),*n,fa);
		start[*n]='\0';
		if ( count != *n ) 
		{
			*ierr = - count -1;
		}
		return;
	}
	sciprint(_("%s: No input file.\n"),"mgetstr");
	*ierr=1;
}
/*--------------------------------------------------------------------------*/
