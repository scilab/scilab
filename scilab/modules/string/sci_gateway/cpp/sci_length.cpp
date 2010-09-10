/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA 2007 - Cong WU
 * Copyright (C) INRIA 2008 - Allan CORNET
 * Copyright (C) DIGITEO 2009 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* desc : For usual or polynomial matrix  n  is the int equal to
   number of rows times number of columns of  M . (Also valid for  M
   a boolean matrix)

   For matrices made of character strings (and in particular for a
   character string)  length  returns in  n  the length of entries of
   the matrix of character strings  M .

   The length of a list is the number of elements in the list
   (also given by  size ).

   length('123')  is  3 .  length([1,2;3,4])  is  4 .                     */
/*------------------------------------------------------------------------*/

#include "alltypes.hxx"
#include "funcmanager.hxx"
#include "string_gw.hxx"

extern "C"
{
#include "core_math.h"
#include "localization.h"
#include "Scierror.h"
}
/*----------------------------------------------------------------------------*/
/* get length */
static Double* lengthStrings(String* _pS);
static Double* lengthMatrix(GenericType* _pG);
static Double* lengthList(List* _pL);
/* !!! WARNING !!! : Read comments about length on sparse matrix */
//static Double lengthSparse(Sparse* _pS);
/*----------------------------------------------------------------------------*/
Function::ReturnValue sci_length(typed_list &in, int _iRetCount, typed_list &out)
{
    Double* pOut = NULL;

    if(in.size() != 1)
    {
        Scierror(999,_("%s: Wrong number of input arguments: %d expected.\n"), "length", 1);
        return Function::Error;
    }

    if(in[0]->isString())
    {
        pOut = lengthStrings(in[0]->getAsString());
    }
    else if(in[0]->isGenericType())
    {
        pOut = lengthMatrix(in[0]->getAsGenericType());
    }
    else if(in[0]->isList())
    {
        pOut = lengthList(in[0]->getAsList());
    }
    //else if(in[0]->isSparse())
    //{
    //}
    else
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument(s).\n"), L"length");
        return Function::Error;
    }

    out.push_back(pOut);
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
static Double* lengthStrings(String* _pS)
{
    if(_pS == NULL)
    {
        return Double::Empty();
    }

    Double* pD = new Double(_pS->rows_get(), _pS->cols_get());
    double* pdblData = pD->real_get();

    for(int i = 0 ; i < _pS->size_get() ; i++)
    {
        pdblData[i] = static_cast<double>(wcslen(_pS->string_get()[i]));
   } 
    return pD;
}
/*--------------------------------------------------------------------------*/
static Double* lengthMatrix(GenericType* _pG)
{
    if(_pG == NULL)
    {
        return Double::Empty();
    }

    return new Double(static_cast<double>(_pG->size_get()));
}
/*--------------------------------------------------------------------------*/
static Double* lengthList(List* _pL)
{
    if(_pL == NULL)
    {
        return Double::Empty();
    }

    return new Double(static_cast<double>(_pL->size_get()));
}
/*--------------------------------------------------------------------------*/
//static Double lengthSparse(Sparse* _pS)
//{
//}
/*--------------------------------------------------------------------------*/
