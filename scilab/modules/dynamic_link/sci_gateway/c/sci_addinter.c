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

/*---------------------------------------------------------------------------*/ 
#include "gw_dynamic_link.h"
#include "stack-c.h"
#include "addinter.h"
#include "localization.h"
#include "dl_genErrorMessage.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
/*-----------------------------------------------------------------------------------*/
int sci_addinter(char *fname,unsigned long fname_len)
{
	CheckRhs(3,3);
	CheckLhs(1,1);

	if ( (VarType(1) == sci_strings) && 
		(VarType(2) == sci_strings) && 
		(VarType(3) == sci_strings) )
	{
		
		char **sharedlibname = NULL;
		char **spname = NULL;
		char **fcts = NULL;
		int nbfcts = 0;
		int ierr = 0;

		int m1 = 0, n1 = 0;
		int m2 = 0, n2 = 0;
		int m3 = 0, n3 = 0;

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&sharedlibname);
		if ( (m1 != n1) && (n1 != 1) )
		{
			freeArrayOfString(sharedlibname, m1*n1);
			Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,1);
			return 0;
		}

		GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&spname);
		GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&m3,&n3,&fcts);

		if ( (m2 != n2) && (n2 != 1) )
		{
			freeArrayOfString(sharedlibname, m1*n1);
			freeArrayOfString(spname, m2*n2);
			freeArrayOfString(fcts, m3*n3);
			Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,2);
			return 0;
		}

		if ( (m3 > 1) &&  (n3 > 1) ) /* check vector string */
		{
			freeArrayOfString(sharedlibname, m1*n1);
			freeArrayOfString(spname, m2*n2);
			freeArrayOfString(fcts, m3*n3);
			Scierror(999,_("%s: Wrong size for input argument #%d: String vector expected.\n"),fname,3);
			return 0;
		}
		
		if ( (m3 == 1) && (n3 >= 1) )
		{
			nbfcts = n3;
		}
		else if ( (n3 == 1) && (m3 >= 1) )
		{
			nbfcts = m3;
		}

		if (nbfcts >= NumberMaxFunctionsByGateway)
		{
			freeArrayOfString(sharedlibname, m1*n1);
			freeArrayOfString(spname, m2*n2);
			freeArrayOfString(fcts, m3*n3);
			Scierror(999,_("%s: Wrong size for input argument #%d: String vector < %d expected.\n"),fname,3,NumberMaxFunctionsByGateway);
			return 0;
		}

		ierr = AddInterfaceToScilab(sharedlibname[0],spname[0],fcts,nbfcts);
		if (ierr == 0)
		{
			LhsVar(1) = 0;
			C2F(putlhsvar)();
		}
		else
		{
			dl_genErrorMessage(fname, ierr, sharedlibname[0]);
		}
		
		freeArrayOfString(sharedlibname, m1*n1);
		freeArrayOfString(spname, m2*n2);
		freeArrayOfString(fcts, m3*n3);
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input arguments: Strings expected.\n"),fname); 
	}
	return 0;
}
/*---------------------------------------------------------------------------*/
