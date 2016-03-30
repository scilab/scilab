/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA 2007 - Cong WU
 * Copyright (C) INRIA 2008 - Allan CORNET
 * Copyright (C) DIGITEO 2009 - Allan CORNET
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

extern "C"
{
#include "core_math.h"
#include "localization.h"
#include "Scierror.h"
}

/*----------------------------------------------------------------------------*/
/* get length */
static types::Double* lengthStrings(types::String* _pS);
static types::Double* lengthMatrix(types::GenericType* _pG);
static types::Double* lengthList(types::List* _pL);
/* !!! WARNING !!! : Read comments about length on sparse matrix */
//static Double lengthSparse(Sparse* _pS);
/*----------------------------------------------------------------------------*/
types::Function::ReturnValue sci_length(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pOut = NULL;

    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "length", 1);
        return types::Function::Error;
    }

    if (in[0]->isString())
    {
        pOut = lengthStrings(in[0]->getAs<types::String>());
    }
    else if (in[0]->isMList())
    {
        //build overload name and check if function exists.
        types::MList* pML = in[0]->getAs<types::MList>();
        std::wstring wst = L"%" + pML->getShortTypeStr() + L"_length";
        symbol::Context* pCtx = symbol::Context::getInstance();
        types::InternalType* pFunc = pCtx->get(symbol::Symbol(wst));
        if (pFunc && pFunc->isCallable())
        {
            //call overload
            Overload::generateNameAndCall(L"length", in, _iRetCount, out);
            return types::Function::OK;
        }

        //MList without overloading, manage like a list
        pOut = lengthList(in[0]->getAs<types::List>());
    }
    else if (in[0]->isList())
    {
        pOut = lengthList(in[0]->getAs<types::List>());
    }
    else if (in[0]->isGenericType())
    {
        pOut = lengthMatrix(in[0]->getAs<types::GenericType>());
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument(s).\n"), "length");
        return types::Function::Error;
    }

    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
static types::Double* lengthStrings(types::String* _pS)
{
    if (_pS == NULL)
    {
        return types::Double::Empty();
    }

    types::Double* pD = new types::Double(_pS->getDims(), _pS->getDimsArray());
    wchar_t** pwst  = _pS->get();
    double* pdbl    = pD->get();

    for (int i = 0 ; i < _pS->getSize() ; i++)
    {
        pdbl[i] = static_cast<double>(wcslen(pwst[i]));
    }
    return pD;
}
/*--------------------------------------------------------------------------*/
static types::Double* lengthMatrix(types::GenericType* _pG)
{
    if (_pG == NULL)
    {
        return types::Double::Empty();
    }

    return new types::Double(static_cast<double>(_pG->getSize()));
}
/*--------------------------------------------------------------------------*/
static types::Double* lengthList(types::List* _pL)
{
    if (_pL == NULL)
    {
        return types::Double::Empty();
    }

    return new types::Double(static_cast<double>(_pL->getSize()));
}
/*--------------------------------------------------------------------------*/
//static Double lengthSparse(Sparse* _pS)
//{
//}
/*--------------------------------------------------------------------------*/
