/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET , Cong WU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* desc : Let  s[k]  stands for the  k  character of Input_StringMatrixings
  ( or the  white space character if  k >length(s) ).
  part  returns  c , a matrix of character Input_StringMatrixings, such that
  c(i,j)  is the Input_StringMatrixing  "s[v(1)]...s[v(n)]"  (  s=mp(i,j)  ).
                                                                          */
/*------------------------------------------------------------------------*/
#include "string_gw.hxx"
#include "funcmanager.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include <string.h>
#include <stdio.h>
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "partfunction.h"
}
/*--------------------------------------------------------------------------*/

using namespace types;

Function::ReturnValue sci_part(typed_list &in, int _iRetCount, typed_list &out)
{
    if (in.size() != 2)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "part", 2);
        return Function::Error;
    }

    if (_iRetCount != -1 && _iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "part", 1);
        return Function::Error;
    }

    //part([], ...
    if (in[0]->isDouble() && in[0]->getAs<Double>()->isEmpty())
    {
        out.push_back(Double::Empty());
        return Function::OK;
    }

    if (in[0]->isString() == false)
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_part";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    String* pS = in[0]->getAs<types::String>();

    if (in[1]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%"  + in[1]->getShortTypeStr() + L"_part";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }


    Double* pD = in[1]->getAs<Double>();
    if (pD->isVector() == false && pD->isEmpty() == false)
    {
        //non vector
        Scierror(999, _("%s: Wrong size for input argument #%d: A vector expected.\n"), "part", 2);
        return Function::Error;
    }

    int* piIndex = new int[pD->getSize()];
    for (int i = 0 ; i < pD->getSize() ; i++)
    {
        piIndex[i] = static_cast<int>(pD->getReal()[i]);
    }

    wchar_t** pwstOut = partfunctionW(pS->get(), pS->getRows(), pS->getCols(), piIndex, pD->getSize());

    String* pOut = new String(pS->getRows(), pS->getCols());
    pOut->set(pwstOut);
    out.push_back(pOut);
    return Function::OK;
    //int i = 0;

    //int m1 = 0, n1 = 0;
    //char **Input_StringMatrix = NULL;
    //int m1n1 = 0; /* m1 * n1 */

    //int m2 = 0, n2 = 0;
    //int StackPosTwo = 0;
    //int *SecondParamaterValue = NULL;
    //int m2n2 = 0; /* m2 * n2 */
    //
    //char **Output_StringMatrix = NULL;

    //CheckRhs(2,2);
    //CheckLhs(1,1);

    //if (VarType(1) == sci_matrix)
    //{
    //	/*Check for an empty matrix */
    //	GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&StackPosTwo);
    //	if (m1 * n1 == 0)
    //	{
    //		LhsVar(1) = 1 ;
    //		C2F(putlhsvar)();
    //		return 0;
    //	}
    //   }
    //if (VarType(1) != sci_strings)
    //{
    //	OverLoad(1);
    //	/*Scierror(999,_("%s: Wrong type for first input argument: String expected.\n"),fname);*/
    //	return 0;
    //}

    //if (VarType(2) != sci_matrix)
    //   {
    //	OverLoad(2);
    //	/*Scierror(999,_("%s: Wrong type for second input argument: Matrix expected.\n"),fname);*/
    //	return 0;
    //   }

    //GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Input_StringMatrix);
    //m1n1 = m1 * n1;
    //
    //GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&StackPosTwo);

    //if ( (m2 == n2) && (n2 == 0) )
    //   {
    //	/* part('something',[]) */
    //	freeArrayOfString(Input_StringMatrix,m1n1);
    //	CreateVarFromPtr(Rhs+1, MATRIX_OF_STRING_DATATYPE, &m1, &n1, NULL);
    //	LhsVar(1)=Rhs+1;
    //	C2F(putlhsvar)();
    //	return 0;
    //}
    //
    //if ( !( (m2 == 1 && n2 > 0) || (m2 > 0 && n2 == 1) ) )
    //{
    //	freeArrayOfString(Input_StringMatrix,m1n1);
    //	Scierror(89,_("%s: Wrong size for input argument #%d.\n"),fname,2);
    //	return 0;
    //}

    //m2n2 = m2 * n2;
    //SecondParamaterValue = istk(StackPosTwo);

    ///* check values of second parameter */
    //for (i = 0;i < m2n2 ;i++)
    //   {
    //	if (SecondParamaterValue[i] < 1)
    //	{
    //		freeArrayOfString(Input_StringMatrix,m1n1);
    //		Scierror(36,_("%s: Wrong values for input argument #%d: Must be >= 1.\n"),fname,2);
    //		return 0;
    //	}
    //   }

    //Output_StringMatrix = partfunction(Input_StringMatrix,m1,n1,SecondParamaterValue,m2n2);

    ///* free pointer */
    //freeArrayOfString(Input_StringMatrix,m1n1);
    //if (Output_StringMatrix == NULL)
    //{
    //	Scierror(999,_("%s: No more memory.\n"),fname);
    //	return 0;
    //}
    ///* put values on stack */
    //CreateVarFromPtr( Rhs+1,MATRIX_OF_STRING_DATATYPE, &m1, &n1, Output_StringMatrix );
    //LhsVar(1) = Rhs+1 ;
    //C2F(putlhsvar)();

    ///* free pointer */
    //freeArrayOfString(Output_StringMatrix,m1n1);

    //return 0;
}
/*--------------------------------------------------------------------------*/
