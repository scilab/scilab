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

#include "function.hxx"
#include "string.hxx"
#include "mlist.hxx"
#include "double.hxx"
#include "funcmanager.hxx"
#include "string_gw.hxx"
#include "context.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include "core_math.h"
#include "localization.h"
#include "Scierror.h"
}

using namespace types;

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

    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "length", 1);
        return Function::Error;
    }

    if (in[0]->isString())
    {
        pOut = lengthStrings(in[0]->getAs<types::String>());
    }
    else if (in[0]->isMList())
    {
        //build overload name and check if function exists.
        MList* pML = in[0]->getAs<MList>();
        std::wstring wst = L"%" + pML->getShortTypeStr() + L"_length";
        symbol::Context* pCtx = symbol::Context::getInstance();
        InternalType* pFunc = pCtx->get(symbol::Symbol(wst));
        if (pFunc && pFunc->isCallable())
        {
            //call overload
            Overload::generateNameAndCall(L"length", in, _iRetCount, out, new ExecVisitor());
            return Function::OK;
        }

        //MList without overloading, manage like a list
        pOut = lengthList(in[0]->getAs<List>());
    }
    else if (in[0]->isList())
    {
        pOut = lengthList(in[0]->getAs<List>());
    }
    else if (in[0]->isGenericType())
    {
        pOut = lengthMatrix(in[0]->getAs<GenericType>());
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument(s).\n"), "length");
        return Function::Error;
    }

    out.push_back(pOut);
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
static Double* lengthStrings(String* _pS)
{
    if (_pS == NULL)
    {
        return Double::Empty();
    }

    Double* pD = new Double(_pS->getDims(), _pS->getDimsArray());
    double* pdblData = pD->getReal();

    for (int i = 0 ; i < _pS->getSize() ; i++)
    {
        pdblData[i] = static_cast<double>(wcslen(_pS->get()[i]));
    }
    return pD;
}
/*--------------------------------------------------------------------------*/
static Double* lengthMatrix(GenericType* _pG)
{
    if (_pG == NULL)
    {
        return Double::Empty();
    }

    return new Double(static_cast<double>(_pG->getSize()));
}
/*--------------------------------------------------------------------------*/
static Double* lengthList(List* _pL)
{
    if (_pL == NULL)
    {
        return Double::Empty();
    }

    return new Double(static_cast<double>(_pL->getSize()));
}
/*--------------------------------------------------------------------------*/
//static Double lengthSparse(Sparse* _pS)
//{
//}
/*--------------------------------------------------------------------------*/
