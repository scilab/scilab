/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Pierre MARECHAL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include "gw_io.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "getrelativefilename.h"
#include "PATH_MAX.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
 /*--------------------------------------------------------------------------*/
/* ================================================================================== */
// sci_getrelativefilename
//
// scilab interface to getrelativefilename
/* ================================================================================== */
int sci_getrelativefilename(char *fname, unsigned long l)
{
	static int l1,n1,m1,l2,n2,m2;

	CheckRhs(2,2);
	CheckLhs(1,1);

	if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) )
	{
		char **param1=NULL;
		char **param2=NULL;
		char *result=(char*)MALLOC(PATH_MAX*sizeof(char));
		
		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&param1);
		
		if ( (n1*m1) != 1 )
		{
			freeArrayOfString(param1,m1*n1);
			Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
			return 0;
		}
		
		GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&param2);
		if ( (n2*m2) !=1 )
		{
			freeArrayOfString(param1,m1*n1);
			freeArrayOfString(param2,m2*n2);
			Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,2);
			return 0;
		}
		
		// make sure the names are not too long
		
		if( strlen(param1[0]) > PATH_MAX )
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: Must be less than %d characters.\n"),fname,1,PATH_MAX);
		}
		
		if( strlen(param2[0]) > PATH_MAX )
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: Must be less than %d characters.\n"),fname,2,PATH_MAX);
		}
		
		result = getrelativefilename(param1[0],param2[0]);
		freeArrayOfString(param1,m1*n1);
		freeArrayOfString(param2,m2*n2);
		
		CreateVarFromPtr(Rhs+3,STRING_DATATYPE,(m1=(int)strlen(result), &m1),&n1,&result);
		LhsVar(1)=Rhs+3;
		if (result) {FREE(result);result=NULL;}
		C2F(putlhsvar)();
	}
	else
	{
		if(GetType(1) != sci_strings)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,2);
		}
	}
	
	return 0;
}
