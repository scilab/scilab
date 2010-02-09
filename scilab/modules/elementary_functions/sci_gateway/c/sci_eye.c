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
/*--------------------------------------------------------------------------*/
#include "gw_elementary_functions.h"
#include "stack-c.h"
#include "basic_functions.h"
#include "api_scilab.h"
#include "api_oldstack.h"

extern int C2F(inteye) (int *id);
/*--------------------------------------------------------------------------*/
int sci_eye(char *fname, int* _piKey)
{
	//SciErr sciErr;
	//int iRows							= 0;
	//int iCols							= 0;
	//int iType 						= 0;

	//int* piAddr1					= NULL;
	//int* piAddr2					= NULL;

	//double *pdblRealRet		= NULL;

	static int id[6];
	C2F(inteye)(id);
	return 0;
	//CheckRhs(0,2);
	//CheckLhs(0,1);


	//if(Rhs <= 0)
	//{
	//	iRows = -1;
	//	iCols = -1;

	//	//special case for operator :
	//	if(Top == 0)
	//	{
	//		Top += 1;
	//		sciErr = allocMatrixOfDouble(pvApiCtx, Top, -1, -1, &pdblRealRet);
	//		if(sciErr.iErr)
	//		{
	//			printError(&sciErr, 0);
	//			return 0;
	//		}

	//		pdblRealRet[0] = 1;
	//		LhsVar(1) = Top;
	//		PutLhsVar();
	//	}
	//	else
	//	{
	//		sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, -1, -1, &pdblRealRet);
	//		if(sciErr.iErr)
	//		{
	//			printError(&sciErr, 0);
	//			return 0;
	//		}

	//		pdblRealRet[0] = 1;
	//		LhsVar(1) = Rhs + 1;
	//		PutLhsVar();
	//	}
	//	return 0;
	//}
	//else if(Rhs == 1)
	//{
	//	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	sciErr = getVarType(pvApiCtx, piAddr1, &iType);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	switch(iType)
	//	{
	//	case sci_matrix :
	//	case sci_poly :
	//	case sci_boolean :
	//	case sci_ints :
	//	case sci_handles :
	//	case sci_strings :
	//		sciErr = getVarDimension(pvApiCtx, piAddr1, &iRows, &iCols);
	//		if(sciErr.iErr)
	//		{
	//			printError(&sciErr, 0);
	//			return 0;
	//		}

	//		break;
	//	default :
	//		OverLoad(1);
	//		return 0;
	//	}

	//}
	//else if(Rhs == 2)
	//{
	//	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	sciErr = getDimFromVar(pvApiCtx, piAddr1, &iRows);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}

	//	sciErr = getDimFromVar(pvApiCtx, piAddr2, &iCols);
	//	if(sciErr.iErr)
	//	{
	//		printError(&sciErr, 0);
	//		return 0;
	//	}
	//}

	//if(iRows == 0 || iCols == 0)
	//{
	//	iRows = 0;
	//	iCols = 0;
	//}
	//else
	//{
	//	iRows = (int)dabss(iRows);
	//	iCols = (int)dabss(iCols);
	//}

	//sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblRealRet);
	//if(sciErr.iErr)
	//{
	//	printError(&sciErr, 0);
	//	return 0;
	//}


	//if(iRows * iCols != 0)
	//{
	//	deyes(pdblRealRet, iRows, iCols);
	//}

	//LhsVar(1) = Rhs + 1;
	//PutLhsVar();
	//return 0;
}
/*--------------------------------------------------------------------------*/
