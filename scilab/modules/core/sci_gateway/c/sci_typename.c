/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
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
#include "gw_core.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
#include "typename.h"
#include "MALLOC.h"
#include "api_scilab.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
static int sci_typename_two_rhs(char *fname,unsigned long fname_len);
static int sci_typename_no_rhs(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
int C2F(sci_typename)(char *fname,unsigned long fname_len)
{
	CheckLhs(1,2); 
	switch (Rhs)
	{
	case 0:
		sci_typename_no_rhs(fname,fname_len);
		break;
	case 2:
		sci_typename_two_rhs(fname,fname_len);
		break;
	default:
		Scierror(999,_("%s: Wrong number of input arguments: %d or %d expected.\n"),fname,0,2);
		break;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
int sci_typename_two_rhs(char *fname,unsigned long fname_len)
{
	SciErr sciErr;
	int m1 = 0, n1 = 0;
	int iType1			= 0;
	int *piAddressVarOne = NULL;
	char *pStVarOne = NULL;
	int lenStVarOne = 0;

	int m2 = 0, n2 = 0;
	int iType2			= 0;
	int *piAddressVarTwo = NULL;
	double *pdVarTwo = NULL;

	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}


	sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}


	sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if ( iType1 != sci_strings )
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
		return 0;
	}

	if ( iType2 != sci_matrix )
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A scalar expected.\n"),fname,2);
		return 0;
	}

	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarTwo,&m2,&n2,&pdVarTwo);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if ( (m2 != n2) && (n2 != 1) ) 
	{
		Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,2);
		return 0;
	}

	sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&m1,&n1,&lenStVarOne,&pStVarOne);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if ( (m1 != n1) && (n1 != 1) ) 
	{
		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
		return 0;
	}
	
	pStVarOne = (char*)MALLOC(sizeof(char)*(lenStVarOne + 1));
	sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&m1,&n1,&lenStVarOne,&pStVarOne);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}


	if (pStVarOne)
	{
		int ierr = 0;
		sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&m1,&n1,&lenStVarOne,&pStVarOne);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		ierr = addNamedType(pStVarOne,(int)pdVarTwo[0]);

		switch(ierr)
		{
		case -1 :
				Scierror(999,_("%s: '%s' already exists.\n"),fname,pStVarOne);
			break;
		case 0:
				LhsVar(1) = 0; 
				C2F(putlhsvar)();
				break;
			break;

		case 1: case 3:
				SciError(224);
			break;

		case 2:
				SciError(225);
			break;

		default:
			/* never here */
			Scierror(999,_("%s: Unknown Error.\n"),fname);
			break;
		}

		FREE(pStVarOne); pStVarOne = NULL;
	}
	else
	{
		Scierror(999,_("%s: No more memory.\n"),fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
int sci_typename_no_rhs(char *fname,unsigned long fname_len)
{
	SciErr sciErr;
	int numberOfTypes = 0;
	int *TypesNumbers = NULL;
	
	int m_out1 = 0, n_out1 = 0;
	
	TypesNumbers = getAllTypesNumber(&numberOfTypes);
	m_out1 = numberOfTypes;
	n_out1 = 1;

	sciErr = createMatrixOfInteger32(pvApiCtx, Rhs + 1,m_out1,n_out1,TypesNumbers);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if (TypesNumbers) { FREE(TypesNumbers);TypesNumbers = NULL;	}
	LhsVar(1) = Rhs + 1; 

	if (Lhs > 1)
	{
		int m_out2 = 0, n_out2 = 0;
		char **TypesNames = NULL;

		TypesNames = getAllTypesName(&numberOfTypes);
		m_out2 = numberOfTypes;
		n_out2 = 1;
		
		sciErr = createMatrixOfString(pvApiCtx, Rhs + 2, m_out2, n_out2, TypesNames);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		if (TypesNames) freeArrayOfString(TypesNames,numberOfTypes);
		LhsVar(2) = Rhs + 2; 
	}
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
