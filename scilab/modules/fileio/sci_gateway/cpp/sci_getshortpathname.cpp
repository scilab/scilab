/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Antoine ELIAS
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

extern "C"
{
#include <string.h>
#include "getshortpathname.h"
#include "sci_malloc.h"
#include "Scierror.h"
#include "localization.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_getshortpathname(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "getshortpathname" , 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1 && _iRetCount != 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "getshortpathname", 1, 2);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "getshortpathname", 1);
        return types::Function::Error;
    }

    types::String* pS = in[0]->getAs<types::String>();

    types::String* pOut1 = new types::String(pS->getRows(), pS->getCols());
    types::Bool* pOut2 = new types::Bool(pS->getRows(), pS->getCols());
    int* pBool = pOut2->get();
    wchar_t** p = pS->get();
    int size = pS->getSize();
    for (int i = 0 ; i < size; i++)
    {
        wchar_t* tmp = getshortpathnameW(p[i], (BOOL*)&pBool[i]);
        pOut1->set(i, tmp);
        FREE(tmp);
    }

    out.push_back(pOut1);
    if (_iRetCount == 2)
    {
        out.push_back(pOut2);
    }
    else
    {
        pOut2->killMe();
    }

    return types::Function::OK;
    //CheckRhs(0,1);
    //CheckLhs(1,2);

    //if (GetType(1) == sci_strings)
    //{
    //	int n1 = 0,m1 = 0, m1n1 = 0;
    //	char **LongNames = NULL;
    //	char **ShortNames = NULL;
    //	BOOL *bOK = NULL;
    //	int i = 0;

    //	GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&LongNames);
    //	m1n1 = m1*n1;

    //	if (m1n1 > 0)
    //	{
    //		ShortNames = (char**) MALLOC(sizeof(char*)* (m1n1));
    //		bOK = (BOOL*) MALLOC(sizeof(BOOL)*(m1n1));

    //		if ( (ShortNames == NULL) || (bOK == NULL) )
    //		{
    //			freeArrayOfString(LongNames, m1n1);
    //			Scierror(999,"%s: Memory allocation error.\n", fname);
    //			return 0;
    //		}
    //	}

    //	for (i = 0;i < m1n1; i++)
    //	{
    //		ShortNames[i] = getshortpathname(LongNames[i], &bOK[i]);
    //	}
    //	freeArrayOfString(LongNames, m1n1);

    //	CreateVarFromPtr( Rhs+1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,ShortNames);
    //	LhsVar(1) = Rhs+1;

    //	if (Lhs == 2)
    //	{
    //		CreateVarFromPtr(Rhs+2,MATRIX_OF_BOOLEAN_DATATYPE, &m1, &n1, &bOK);
    //		LhsVar(2) = Rhs + 2;
    //	}

    //	C2F(putlhsvar)();

    //	freeArrayOfString(ShortNames, m1n1);
    //	if (bOK) {FREE(bOK); bOK = NULL;}
    //}
    //else
    //{
    //	Scierror(999,_("%s: Wrong type for input argument: string expected.\n"),fname);
    //}
    //return 0;
}
/*--------------------------------------------------------------------------*/
