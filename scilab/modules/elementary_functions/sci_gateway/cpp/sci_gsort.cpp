/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Cedric DELAMARRE
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 * Copyright (C) 2018 - Samuel GOUGEON
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
#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "overload.hxx"
#include "gsort.hxx"
#include "context.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_gsort(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    // In all cases, to later test in[0]:
    if (in.size() < 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): At least %d expected.\n"), "gsort", 1);
        return types::Function::Error;
    }
    // The maximal number of input args may depend on the input data type, due to specific options

    //
    // Special cases
    //
    if (in[0]->isGenericType() == false)
    {
        // custom types
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_gsort";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }
    types::GenericType* pGTIn = in[0]->getAs<types::GenericType>();
    if (pGTIn->getDims() > 2)
    {
        // hypermatrix
        return Overload::call(L"%hm_gsort", in, _iRetCount, out);
    }
    if (pGTIn->isSparse())
    {
        // sparse
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_gsort";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }
    if (pGTIn->isComplex() && symbol::Context::getInstance()->getFunction(symbol::Symbol(L"%_gsort")))
    {
        // complex is documented as being managed through overloading
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_gsort";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    //
    // Common case
    //

    if (in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "gsort", 1, 3);
        return types::Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "gsort", 1, 2);
        return types::Function::Error;
    }

    // Get the sorting order
    std::wstring wstrWay = L"d";
    if (in.size() > 2)
    {
        if (in[2]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "gsort", 3);
            return types::Function::Error;
        }

        wstrWay = in[2]->getAs<types::String>()->get(0);
        if (wstrWay != L"i" && wstrWay != L"d")
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: %s expected.\n"), "gsort", 3, "'i'|'d'");
            return types::Function::Error;
        }
    }

    // Get the sorting method
    std::wstring wstrProcess = L"g";
    if (in.size() >= 2)
    {
        if (in[1]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "gsort", 2);
            return types::Function::Error;
        }

        wstrProcess = in[1]->getAs<types::String>()->get(0);

        if ( wstrProcess != L"c"  &&
                wstrProcess != L"r"  &&
                wstrProcess != L"g"  &&
                wstrProcess != L"lc" &&
                wstrProcess != L"lr")
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: ['g' 'r' 'c' 'lc' 'lr'] expected.\n"), "gsort", 2);
            return types::Function::Error;
        }
    }

    // Get data and perform operation for each types::
    types::Double* pDblInd = NULL;
    if (_iRetCount == 2)
    {
        int iRowsInd = (wstrProcess == L"lc") ? 1 : pGTIn->getRows();
        int iColsInd = (wstrProcess == L"lr") ? 1 : pGTIn->getCols();

        pDblInd = new types::Double(iRowsInd, iColsInd);
    }

    if (in[0]->isDouble()) // double
    {
        types::Double* pDblIn = in[0]->getAs<types::Double>();
        types::Double* pDblOut = gsort(pDblIn, pDblInd, wstrWay, wstrProcess);
        out.push_back(pDblOut);
    }
    else if (in[0]->isString()) // string
    {
        types::String* pStringIn = in[0]->getAs<types::String>();
        types::String* pStringOut = gsort(pStringIn, pDblInd, wstrWay, wstrProcess);
        out.push_back(pStringOut);
    }
    else if (in[0]->isInt8()) // int
    {
        types::Int8* pIIn = in[0]->getAs<types::Int8>();
        types::Int8* pIOut = gsort(pIIn, pDblInd, wstrWay, wstrProcess);
        out.push_back(pIOut);
    }
    else if (in[0]->isInt16())
    {
        types::Int16* pIIn = in[0]->getAs<types::Int16>();
        types::Int16* pIOut = gsort(pIIn, pDblInd, wstrWay, wstrProcess);
        out.push_back(pIOut);
    }
    else if (in[0]->isInt32())
    {
        types::Int32* pIIn = in[0]->getAs<types::Int32>();
        types::Int32* pIOut = gsort(pIIn, pDblInd, wstrWay, wstrProcess);
        out.push_back(pIOut);
    }
    else if (in[0]->isInt64())
    {
        types::Int64* pIIn = in[0]->getAs<types::Int64>();
        types::Int64* pIOut = gsort(pIIn, pDblInd, wstrWay, wstrProcess);
        out.push_back(pIOut);
    }
    else if (in[0]->isUInt8()) // uint
    {
        types::UInt8* pIIn = in[0]->getAs<types::UInt8>();
        types::UInt8* pIOut = gsort(pIIn, pDblInd, wstrWay, wstrProcess);
        out.push_back(pIOut);
    }
    else if (in[0]->isUInt16())
    {
        types::UInt16* pIIn = in[0]->getAs<types::UInt16>();
        types::UInt16* pIOut = gsort(pIIn, pDblInd, wstrWay, wstrProcess);
        out.push_back(pIOut);
    }
    else if (in[0]->isUInt32())
    {
        types::UInt32* pIIn = in[0]->getAs<types::UInt32>();
        types::UInt32* pIOut = gsort(pIIn, pDblInd, wstrWay, wstrProcess);
        out.push_back(pIOut);
    }
    else if (in[0]->isUInt64())
    {
        types::UInt64* pIIn = in[0]->getAs<types::UInt64>();
        types::UInt64* pIOut = gsort(pIIn, pDblInd, wstrWay, wstrProcess);
        out.push_back(pIOut);
    }
    else    // Other generic data types not supported
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_gsort";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    // Returns indices when requested
    if (_iRetCount == 2)
    {
        out.push_back(pDblInd);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
