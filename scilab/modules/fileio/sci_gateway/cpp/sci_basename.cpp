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
#include "bool.hxx"
#include "double.hxx"

extern "C"
{
#include "localization.h"
#include "basename.h"
#include "Scierror.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_basename(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iExpand     = 1;

    if (in.size() < 1 || in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "basename", 1, 3);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "basename", 1);
        return types::Function::Error;
    }

    if (in.size() > 2)
    {
        if (in[2]->isBool() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), "basename", 3);
            return types::Function::Error;
        }

        if (in[2]->getAs<types::Bool>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar boolean expected.\n"), "basename", 3);
            return types::Function::Error;
        }

        iExpand = in[2]->getAs<types::Bool>()->get()[0];
    }

    if (in.size() > 1)
    {
        if (in[1]->isBool() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), "basename", 2);
            return types::Function::Error;
        }

        if (in[1]->getAs<types::Bool>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar boolean expected.\n"), "basename", 2);
            return types::Function::Error;
        }
    }

    if (in[0]->isDouble() && in[0]->getAs<types::Double>()->isEmpty())
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }
    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string matrix expected.\n"), "basename", 1);
        return types::Function::Error;
    }

    types::String* pS = in[0]->getAs<types::String>();
    types::String* pOut = new types::String(pS->getRows(), pS->getCols());
    for (int i = 0 ; i < pS->getSize() ; i++)
    {
        wchar_t* base = basenameW(pS->get(i), (BOOL)iExpand);
        pOut->set(i, base);
        FREE(base);
    }


    out.push_back(pOut);
    return types::Function::OK;
    //SciErr sciErr;
    //BOOL flag = TRUE; /* default */
    //BOOL flagexpand = TRUE; /* default */

    //int *piAddressVarOne = NULL;
    //wchar_t **pStVarOne = NULL;
    //int *lenStVarOne = NULL;
    //int iType1					= 0;
    //int m1 = 0, n1 = 0;

    //wchar_t **pStResult = NULL;

    //int i = 0;

    ///* Check Input & Output parameters */
    //CheckRhs(1,3);
    //CheckLhs(1,1);

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

    //	sciErr = getMatrixOfBoolean(pvApiCtx, piAddressVarThree, &m3, &n3,  &piData);
    //	if(sciErr.iErr)
    //	{
    //		printError(&sciErr, 0);
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

    //	flag = piData[0];
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
    //	int i = 0;

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

    //	// get lenStVarOne value
    //	sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
    //	if(sciErr.iErr)
    //	{
    //		freeArrayOfWideString(pStVarOne, m1 * n1);
    //		if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
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

    //	for (i = 0; i < (m1 * n1); i++)
    //	{
    //		pStVarOne[i] = (wchar_t*)MALLOC(sizeof(wchar_t) * (lenStVarOne[i] + 1));
    //		if (pStVarOne[i] == NULL)
    //		{
    //			freeArrayOfWideString(pStVarOne, m1 * n1);
    //			if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
    //			Scierror(999,_("%s: Memory allocation error.\n"),fname);
    //			return 0;
    //		}
    //	}

    //	// get pStVarOne
    //	sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
    //	if(sciErr.iErr)
    //	{
    //		freeArrayOfWideString(pStVarOne, m1 * n1);
    //		if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
    //		printError(&sciErr, 0);
    //		return 0;
    //	}

    //	pStResult = (wchar_t**)MALLOC(sizeof(wchar_t*) * (m1 * n1));

    //	if (pStResult == NULL)
    //	{
    //		if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
    //		Scierror(999,_("%s: Memory allocation error.\n"),fname);
    //		return 0;
    //	}

    //	for (i=0;i< m1 * n1; i++)
    //	{
    //		pStResult[i] = basenameW(pStVarOne[i], flagexpand);
    //	}

    //	sciErr = createMatrixOfWideString(pvApiCtx, Rhs + 1, m1, n1, pStResult);
    //	if(sciErr.iErr)
    //	{
    //		printError(&sciErr, 0);
    //		return 0;
    //	}

    //	LhsVar(1) = Rhs + 1;
    //	C2F(putlhsvar)();

    //	if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
    //	freeArrayOfWideString(pStResult, m1 * n1);
    //	freeArrayOfWideString(pStVarOne, m1 * n1);
    //}
    //else
    //{
    //	Scierror(999,_("%s: Wrong type for input argument #%d: String array expected.\n"), fname, 1);
    //}
    //return 0;
}
/*--------------------------------------------------------------------------*/
