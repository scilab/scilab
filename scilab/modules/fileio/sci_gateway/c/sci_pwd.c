/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "gw_fileio.h"
#include "stack-c.h"
#include "scicurdir.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "api_scilab.h"
#include "api_oldstack.h"

/*--------------------------------------------------------------------------*/
int sci_pwd(char *fname,int *_piKey)
{
    SciErr sciErr;
	int ierr = 0;
	char *path = NULL;

	CheckRhs(0,0);
	CheckLhs(0,1);

	path = scigetcwd(&ierr);

	if (ierr)
	{
		if (path) {FREE(path); path = NULL;}
		Scierror(998,_("%s: An error occurred.\n"), fname);
		return 0;
	}
	else
	{
		int n1 = 1;
		int m1 = (int)strlen(path);

		n1 = 1;
        sciErr = createMatrixOfString(_piKey, Rhs + 1, 1, 1, &path);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

		LhsVar(1) = Rhs+1;
		PutLhsVar();

		if (path) {FREE(path); path = NULL;}
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
