/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Allan CORNET
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "gw_fileio.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "PATH_MAX.h"
#include "fullpath.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
int sci_fullpath(char *fname,unsigned long fname_len)
{
	Rhs=Max(Rhs,0);
	CheckRhs(1,1) ;
	CheckLhs(0,1) ;

	if (GetType(1) == sci_strings)
	{
		static int l1,n1,m1;
		char *relPath = NULL;
		char fullpath[PATH_MAX*4];
		char *returnedPath = NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		/* Bug 3089 */
		relPath = cstk(l1);

		if( get_full_path( fullpath, relPath, PATH_MAX*4 ) != NULL )
		{
			char *Output = strdup(fullpath);

			n1=1;
			CreateVarFromPtr( Rhs+1,STRING_DATATYPE,(m1=(int)strlen(Output), &m1),&n1,&Output);
			LhsVar(1) = Rhs+1;
			C2F(putlhsvar)();	

			if (Output) {FREE(Output);Output=NULL;}
		}
		else
		{
			Scierror(999,_("%s: Wrong value for input argument #%d: '%s' is an invalid path.\n"),fname,1,relPath);
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
	}

	return 0;
}
/*--------------------------------------------------------------------------*/

