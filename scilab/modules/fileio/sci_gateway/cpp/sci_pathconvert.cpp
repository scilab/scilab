/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/
/*--------------------------------------------------------------------------*/
#include "fileio_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"
#include "bool.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
#include "pathconvert.h"
}

#define UNIX_TYPE       L"u"
#define WINDOWS_TYPE    L"w"
#define _UNIX_TYPE      "u"
#define _WINDOWS_TYPE   "w"

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_pathconvert(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    PathConvertType PType   = AUTO_STYLE;
    int iPathExpand         = 1;
    int iPathTrail          = 1;

    if (in.size() < 1 || in.size() > 4)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), "pathconvert" , 1, 4);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "pathconvert", 1);
        return types::Function::Error;
    }

    //get type
    if (in.size() > 3)
    {
        if (in[3]->isString() == false || in[3]->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "pathconvert", 4);
            return types::Function::Error;
        }

        wchar_t* pwstType = in[3]->getAs<types::String>()->get(0);
        if (wcscmp(pwstType, WINDOWS_TYPE) == 0)
        {
            PType = WINDOWS_STYLE;
        }
        else if (wcscmp(pwstType, UNIX_TYPE) == 0)
        {
            PType = UNIX_STYLE;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), "pathconvert", 4, _UNIX_TYPE, _WINDOWS_TYPE);
            return types::Function::Error;
        }
    }

    if (in.size() > 2)
    {
        if (in[2]->isBool() == false || in[2]->getAs<types::Bool>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), "pathconvert", 3);
            return types::Function::Error;
        }

        iPathExpand = in[2]->getAs<types::Bool>()->get()[0];
    }

    if (in.size() > 1)
    {
        if (in[1]->isBool() == false || in[1]->getAs<types::Bool>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), "pathconvert", 2);
            return types::Function::Error;
        }

        iPathTrail = in[1]->getAs<types::Bool>()->get()[0];
    }

    if (in[0]->isDouble() && in[0]->getAs<types::Double>()->isEmpty())
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "pathconvert", 1);
        return types::Function::Error;
    }

    types::String* pS = in[0]->getAs<types::String>();
    types::String* pOut = new types::String(pS->getRows(), pS->getCols());
    wchar_t** pStr = pOut->get();


    for (int i = 0 ; i < pS->getSize() ; i++)
    {
        pStr[i] = pathconvertW(pS->get(i), (BOOL) iPathTrail, (BOOL) iPathExpand, PType);
    }

    out.push_back(pOut);
    return types::Function::OK;
    //SciErr sciErr;
    //int *piAddressVarOne = NULL;
    //wchar_t **pStVarOne = NULL;
    //int iType1 = 0;
    //int *lenStVarOne = NULL;
    //int m1 = 0, n1 = 0;

    //wchar_t **results = NULL;
    //int i = 0;

    //BOOL flagtrail = TRUE;
    //BOOL flagexpand = TRUE;

    //PathConvertType PType = AUTO_STYLE;

    ///* Check Input & Output parameters */
    //CheckRhs(1,4);
    //CheckLhs(1,1);


    //if (Rhs > 3)
    //{
    //	int *piAddressVarFour = NULL;
    //	wchar_t *pStVarFour = NULL;
    //	int iType4 = 0;
    //	int lenStVarFour = 0;
    //	int m4 = 0, n4 = 0;

    //	sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddressVarFour);
    //	if(sciErr.iErr)
    //	{
    //		printError(&sciErr, 0);
    //		return 0;
    //	}

    //	sciErr = getVarType(pvApiCtx, piAddressVarFour, &iType4);
    //	if(sciErr.iErr)
    //	{
    //		printError(&sciErr, 0);
    //		return 0;
    //	}

    //	if (iType4 != sci_strings)
    //	{
    //		Scierror(999,_("%s: Wrong type for input argument #%d: string expected.\n"), fname, 4);
    //		return 0;
    //	}

    //	sciErr = getVarDimension(pvApiCtx, piAddressVarFour, &m4, &n4);
    //	if(sciErr.iErr)
    //	{
    //		printError(&sciErr, 0);
    //		return 0;
    //	}

    //	if ( (m4 != n4) && (n4 != 1) )
    //	{
    //		Scierror(999,_("%s: Wrong size for input argument #%d: string expected.\n"), fname, 4);
    //		return 0;
    //	}

    //	sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarFour, &m4, &n4, &lenStVarFour, &pStVarFour);
    //	if(sciErr.iErr)
    //	{
    //		printError(&sciErr, 0);
    //		return 0;
    //	}

    //	pStVarFour = (wchar_t*)MALLOC(sizeof(wchar_t)*(lenStVarFour + 1));
    //	if (pStVarFour == NULL)
    //	{
    //		Scierror(999,_("%s: Memory allocation error.\n"),fname);
    //		return 0;
    //	}

    //	sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarFour, &m4, &n4, &lenStVarFour, &pStVarFour);
    //	if(sciErr.iErr)
    //	{
    //		printError(&sciErr, 0);
    //		return 0;
    //	}


    //	if ( (wcscmp(pStVarFour, L"w") == 0) || (wcscmp(pStVarFour, L"u") == 0) )
    //	{
    //		if (wcscmp(pStVarFour, L"w") == 0)
    //		{
    //			PType = WINDOWS_STYLE;
    //		}
    //		else
    //		{
    //			PType = UNIX_STYLE;
    //		}
    //		if (pStVarFour) {FREE(pStVarFour); pStVarFour = NULL;}
    //	}
    //	else
    //	{
    //		if (pStVarFour) {FREE(pStVarFour); pStVarFour = NULL;}
    //		Scierror(999,_("%s: Wrong value for input argument #%d: ''w'' or ''u'' string expected.\n"), fname, 4);
    //		return 0;
    //	}
    //}

    //if (Rhs > 2)
    //{
    //	int *piAddressVarThree = NULL;
    //	int *piData = NULL;
    //	int iType3	= 0;
    //	int m3 = 0, n3 = 0;

    //	sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
    //	if(sciErr.iErr)
    //	{
    //		printError(&sciErr, 0);
    //		return 0;
    //	}

    //	sciErr = getVarType(pvApiCtx, piAddressVarThree, &iType3);
    //	if(sciErr.iErr)
    //	{
    //		printError(&sciErr, 0);
    //		return 0;
    //	}

    //	if (iType3 != sci_boolean)
    //	{
    //		Scierror(999,_("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 3);
    //		return 0;
    //	}

    //	sciErr = getVarDimension(pvApiCtx, piAddressVarThree, &m3, &n3);
    //	if(sciErr.iErr)
    //	{
    //		printError(&sciErr, 0);
    //		return 0;
    //	}


    //	if ( (m3 != n3) && (n3 != 1) )
    //	{
    //		Scierror(999,_("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 3);
    //		return 0;
    //	}

    //	sciErr = getMatrixOfBoolean(pvApiCtx, piAddressVarThree, &m3, &n3,  &piData);
    //	if(sciErr.iErr)
    //	{
    //		printError(&sciErr, 0);
    //		return 0;
    //	}

    //	flagexpand = piData[0];
    //}

    //if (Rhs > 1)
    //{
    //	int *piAddressVarTwo = NULL;
    //	int *piData = NULL;
    //	int iType2	= 0;
    //	int m2 = 0, n2 = 0;

    //	sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    //	if(sciErr.iErr)
    //	{
    //		printError(&sciErr, 0);
    //		return 0;
    //	}

    //	sciErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
    //	if(sciErr.iErr)
    //	{
    //		printError(&sciErr, 0);
    //		return 0;
    //	}

    //	if (iType2 != sci_boolean)
    //	{
    //		Scierror(999,_("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 2);
    //		return 0;
    //	}

    //	sciErr = getVarDimension(pvApiCtx, piAddressVarTwo, &m2, &n2);
    //	if(sciErr.iErr)
    //	{
    //		printError(&sciErr, 0);
    //		return 0;
    //	}

    //	if ( (m2 != n2) && (n2 != 1) )
    //	{
    //		Scierror(999,_("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 2);
    //		return 0;
    //	}

    //	sciErr = getMatrixOfBoolean(pvApiCtx, piAddressVarTwo, &m2, &n2,  &piData);
    //	if(sciErr.iErr)
    //	{
    //		printError(&sciErr, 0);
    //		return 0;
    //	}

    //	flagtrail = piData[0];
    //}

    //sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    //if(sciErr.iErr)
    //{
    //	printError(&sciErr, 0);
    //	return 0;
    //}

    //sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
    //if(sciErr.iErr)
    //{
    //	printError(&sciErr, 0);
    //	return 0;
    //}

    //if (iType1 == sci_matrix)
    //{
    //	sciErr = getVarDimension(pvApiCtx, piAddressVarOne, &m1, &n1);
    //	if(sciErr.iErr)
    //	{
    //		printError(&sciErr, 0);
    //		return 0;
    //	}

    //	if ( (m1 == n1) && (m1 == 0) )
    //	{
    //		sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m1, n1, NULL);
    //		if(sciErr.iErr)
    //		{
    //			printError(&sciErr, 0);
    //			return 0;
    //		}

    //		LhsVar(1) = Rhs + 1;
    //		C2F(putlhsvar)();
    //	}
    //	else
    //	{
    //		Scierror(999,_("%s: Wrong type for input argument #%d: String array expected.\n"), fname, 1);
    //	}
    //}
    //else if (iType1 == sci_strings)
    //{
    //	sciErr = getVarDimension(pvApiCtx, piAddressVarOne, &m1, &n1);
    //	if(sciErr.iErr)
    //	{
    //		printError(&sciErr, 0);
    //		return 0;
    //	}

    //	lenStVarOne = (int*)MALLOC(sizeof(int) * (m1 * n1));
    //	if (lenStVarOne == NULL)
    //	{
    //		Scierror(999,_("%s: Memory allocation error.\n"),fname);
    //		return 0;
    //	}

    //	results = (wchar_t **)MALLOC(sizeof(wchar_t*) * (m1 * n1));

    //	if (results == NULL)
    //	{
    //		if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
    //		freeArrayOfWideString(pStVarOne, m1 * n1);
    //		Scierror(999,_("%s: Memory allocation error.\n"),fname);
    //		return 0;
    //	}

    //	sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
    //	if(sciErr.iErr)
    //	{
    //		printError(&sciErr, 0);
    //		return 0;
    //	}

    //	pStVarOne = (wchar_t**)MALLOC(sizeof(wchar_t*) * (m1 * n1));
    //	if (pStVarOne == NULL)
    //	{
    //		if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
    //		Scierror(999,_("%s: Memory allocation error.\n"),fname);
    //		return 0;
    //	}

    //	for( i = 0; i < m1 * n1; i++)
    //	{
    //		pStVarOne[i] = (wchar_t*)MALLOC(sizeof(wchar_t)*(lenStVarOne[i] + 1));
    //		if (pStVarOne[i] == NULL)
    //		{
    //			if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
    //			freeArrayOfWideString(pStVarOne, m1 * n1);
    //			Scierror(999,_("%s: Memory allocation error.\n"),fname);
    //			return 0;
    //		}
    //	}

    //	sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
    //	if(sciErr.iErr)
    //	{
    //		printError(&sciErr, 0);
    //		return 0;
    //	}

    //	for( i = 0; i < m1 * n1; i++)
    //	{
    //		results[i] = pathconvertW(pStVarOne[i], flagtrail, flagexpand, PType);
    //	}

    //	sciErr = createMatrixOfWideString(pvApiCtx, Rhs + 1, m1, n1, results);
    //	if(sciErr.iErr)
    //	{
    //		printError(&sciErr, 0);
    //		return 0;
    //	}

    //	LhsVar(1) = Rhs + 1;
    //	C2F(putlhsvar)();

    //	if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
    //	freeArrayOfWideString(results, m1 * n1);
    //	freeArrayOfWideString(pStVarOne, m1 * n1);
    //}
    //else
    //{
    //	Scierror(999,_("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
    //	return 0;
    //}
    //return 0;
}
/*--------------------------------------------------------------------------*/
