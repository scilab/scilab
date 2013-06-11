/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#include "fileio_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "bool.hxx"

extern "C"
{
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "FileExist.h"
#include "isdir.h"
#include "expandPathVariable.h"
}

/*--------------------------------------------------------------------------*/

using namespace types;

Function::ReturnValue sci_isfile(typed_list &in, int _iRetCount, typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "isfile" , 1);
        return Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "isfile", 1);
        return Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "isfile", 1);
        return Function::Error;
    }

    String* pS  = in[0]->getAs<types::String>();
    Bool* pOut  = new Bool(pS->getRows(), pS->getCols());

    for (int i = 0 ; i < pS->getSize() ; i++)
    {
        wchar_t *expandedPath = expandPathVariableW(pS->get(i));
        if (expandedPath)
        {
            pOut->get()[i] = !isdirW(expandedPath) && FileExistW(expandedPath);
            FREE(expandedPath);
            expandedPath = NULL;
        }
        else
        {
            pOut->get()[i] = FALSE;
        }
    }

    out.push_back(pOut);
    return Function::OK;
    //SciErr sciErr;
    //int *piAddressVarOne = NULL;
    //wchar_t **pStVarOne = NULL;
    //int iType = 0;
    //int *lenStVarOne = NULL;
    //int m1 = 0, n1 = 0;

    //BOOL *results = NULL;
    //int m_out = 0, n_out = 0;
    //int i = 0;

    ///* Check Input & Output parameters */
    //CheckRhs(1,1);
    //CheckLhs(1,1);

    //sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    //if(sciErr.iErr)
    //{
    //	printError(&sciErr, 0);
    //	return 0;
    //}

    //sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
    //if(sciErr.iErr)
    //{
    //	printError(&sciErr, 0);
    //	return 0;
    //}

    //if (iType != sci_strings)
    //{
    //	Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
    //	return 0;
    //}

    //sciErr = getVarDimension(pvApiCtx, piAddressVarOne, &m1, &n1);
    //if(sciErr.iErr)
    //{
    //	printError(&sciErr, 0);
    //	return 0;
    //}


    //lenStVarOne = (int*)MALLOC(sizeof(int) * (m1 * n1));
    //if (lenStVarOne == NULL)
    //{
    //	Scierror(999,_("%s: Memory allocation error.\n"),fname);
    //	return 0;
    //}

    //results = (BOOL*)MALLOC(sizeof(BOOL) * (m1 * n1));
    //if (results == NULL)
    //{
    //	if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
    //	freeArrayOfWideString(pStVarOne, m1 * n1);
    //	Scierror(999,_("%s: Memory allocation error.\n"),fname);
    //	return 0;
    //}

    //sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
    //if(sciErr.iErr)
    //{
    //	printError(&sciErr, 0);
    //	return 0;
    //}

    //pStVarOne = (wchar_t**)MALLOC(sizeof(wchar_t*) * (m1 * n1));
    //if (pStVarOne == NULL)
    //{
    //	if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
    //	Scierror(999,_("%s: Memory allocation error.\n"),fname);
    //	return 0;
    //}

    //for (i=0;i< m1 * n1; i++)
    //{
    //	pStVarOne[i] = (wchar_t*)MALLOC(sizeof(wchar_t)* (lenStVarOne[i] + 1));
    //	if (pStVarOne[i] == NULL)
    //	{
    //		freeArrayOfWideString(pStVarOne, m1 * n1);
    //		if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
    //		Scierror(999,_("%s: Memory allocation error.\n"),fname);
    //		return 0;
    //	}
    //}

    //sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
    //if(sciErr.iErr)
    //{
    //	printError(&sciErr, 0);
    //	return 0;
    //}

    //for (i=0;i< m1 * n1; i++)
    //{
    //	wchar_t *expandedPath = expandPathVariableW(pStVarOne[i]);
    //	if (expandedPath)
    //	{
    //		results[i] = !isdirW(expandedPath) && FileExistW(expandedPath);
    //		FREE(expandedPath); expandedPath = NULL;
    //	}
    //	else
    //	{
    //		results[i] = FALSE;
    //	}
    //}

    //if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
    //freeArrayOfWideString(pStVarOne, m1 * n1);

    //sciErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, m1, n1, results);
    //if(sciErr.iErr)
    //{
    //	printError(&sciErr, 0);
    //	return 0;
    //}

    //LhsVar(1) = Rhs + 1;

    //if (results) {FREE(results); results = NULL;}
    //
    //C2F(putlhsvar)();
    //return 0;
}
/*--------------------------------------------------------------------------*/
