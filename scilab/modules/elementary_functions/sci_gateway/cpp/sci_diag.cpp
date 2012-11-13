/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"
#include "diag.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_diag(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblIn       = NULL;
    types::Polynom* pPolyIn     = NULL;
    types::String* pStrIn       = NULL;

    types::Double* pDblOut      = NULL;
    types::Polynom* pPolyOut    = NULL;
    types::String* pStrOut      = NULL;

    int iStartPos = 0;

    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "diag", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "diag", 1);
        return types::Function::Error;
    }

    /***** get data *****/
    if (in[0]->isDouble())
    {
        pDblIn = in[0]->getAs<types::Double>();

        if (pDblIn->getDims() > 2)
        {
            std::wstring wstFuncName = L"%hm_diag";
            return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
        }
    }
    else if (in[0]->isString())
    {
        pStrIn = in[0]->getAs<types::String>();

        if (pStrIn->getDims() > 2)
        {
            std::wstring wstFuncName = L"%hm_diag";
            return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
        }
    }
    else if (in[0]->isBool()) // bool
    {
        types::Bool* pBIn = in[0]->getAs<types::Bool>();
        pDblIn = new types::Double(pBIn->getDims(), pBIn->getDimsArray());
        for (int i = 0; i < pBIn->getSize(); i++)
        {
            pBIn->get(i) ? pDblIn->set(i, 1) : pDblIn->set(i, 0);
        }
    }
    else if (in[0]->isPoly())
    {
        pPolyIn = in[0]->getAs<types::Polynom>();
        if (pPolyIn->getDims() > 2)
        {
            std::wstring wstFuncName = L"%hm_diag";
            return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
        }
    }
    else if (in[0]->isInt8()) // int
    {
        types::Int8* pIIn = in[0]->getAs<types::Int8>();
        pDblIn = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
        for (int i = 0; i < pIIn->getSize(); i++)
        {
            pDblIn->set(i, static_cast<double>(pIIn->get(i)));
        }
    }
    else if (in[0]->isInt16())
    {
        types::Int16* pIIn = in[0]->getAs<types::Int16>();
        pDblIn = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
        for (int i = 0; i < pIIn->getSize(); i++)
        {
            pDblIn->set(i, static_cast<double>(pIIn->get(i)));
        }
    }
    else if (in[0]->isInt32())
    {
        types::Int32* pIIn = in[0]->getAs<types::Int32>();
        pDblIn = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
        for (int i = 0; i < pIIn->getSize(); i++)
        {
            pDblIn->set(i, static_cast<double>(pIIn->get(i)));
        }
    }
    else if (in[0]->isInt64()) // uint
    {
        types::Int64* pIIn = in[0]->getAs<types::Int64>();
        pDblIn = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
        for (int i = 0; i < pIIn->getSize(); i++)
        {
            pDblIn->set(i, static_cast<double>(pIIn->get(i)));
        }
    }
    else if (in[0]->isUInt8())
    {
        types::Int8* pIIn = in[0]->getAs<types::Int8>();
        pDblIn = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
        for (int i = 0; i < pIIn->getSize(); i++)
        {
            pDblIn->set(i, static_cast<double>(pIIn->get(i)));
        }
    }
    else if (in[0]->isUInt16())
    {
        types::Int16* pIIn = in[0]->getAs<types::Int16>();
        pDblIn = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
        for (int i = 0; i < pIIn->getSize(); i++)
        {
            pDblIn->set(i, static_cast<double>(pIIn->get(i)));
        }
    }
    else if (in[0]->isUInt32())
    {
        types::Int32* pIIn = in[0]->getAs<types::Int32>();
        pDblIn = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
        for (int i = 0; i < pIIn->getSize(); i++)
        {
            pDblIn->set(i, static_cast<double>(pIIn->get(i)));
        }
    }
    else if (in[0]->isUInt64())
    {
        types::Int64* pIIn = in[0]->getAs<types::Int64>();
        pDblIn = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
        for (int i = 0; i < pIIn->getSize(); i++)
        {
            pDblIn->set(i, static_cast<double>(pIIn->get(i)));
        }
    }
    else
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_diag";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    if (in.size() == 2)
    {
        if (in[1]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), "diag", 2);
            return types::Function::Error;
        }

        types::Double* pDbl = in[1]->getAs<types::Double>();

        if (pDbl->isScalar() == false || pDbl->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), "diag", 2);
            return types::Function::Error;
        }

        iStartPos = static_cast<int>(pDbl->get(0));
    }

    /***** perform operation *****/
    if (pDblIn)
    {
        pDblOut = diag(pDblIn, iStartPos);
    }
    else if (pPolyIn)
    {
        pPolyOut = diag(pPolyIn, iStartPos);
        if (pPolyOut == NULL)
        {
            out.push_back(types::Double::Empty());
            return types::Function::OK;
        }
    }
    else if (pStrIn)
    {
        pStrOut = diag(pStrIn, iStartPos);
        if (pStrOut == NULL)
        {
            out.push_back(types::Double::Empty());
            return types::Function::OK;
        }
    }

    /***** set result *****/
    if (in[0]->isDouble())
    {
        out.push_back(pDblOut);
    }
    else if (in[0]->isBool())
    {
        types::Bool* pBOut = new types::Bool(pDblOut->getDims(), pDblOut->getDimsArray());
        for (int i = 0; i < pDblOut->getSize(); i++)
        {
            pDblOut->get(i) ? pBOut->set(i, 1) : pBOut->set(i, 0);
        }
        out.push_back(pBOut);
        delete pDblOut;
    }
    else if (in[0]->isPoly())
    {
        out.push_back(pPolyOut);
    }
    else if (in[0]->isString())
    {
        out.push_back(pStrOut);
    }
    else if (in[0]->isInt8())
    {
        types::Int8* pIOut = new types::Int8(pDblOut->getDims(), pDblOut->getDimsArray());
        for (int i = 0; i < pIOut->getSize(); i++)
        {
            pIOut->set(i, static_cast<char>(pDblOut->get(i)));
        }
        out.push_back(pIOut);
        delete pDblOut;
    }
    else if (in[0]->isInt16())
    {
        types::Int16* pIOut = new types::Int16(pDblOut->getDims(), pDblOut->getDimsArray());
        for (int i = 0; i < pIOut->getSize(); i++)
        {
            pIOut->set(i, static_cast<short int>(pDblOut->get(i)));
        }
        out.push_back(pIOut);
        delete pDblOut;
    }
    else if (in[0]->isInt32())
    {
        types::Int32* pIOut = new types::Int32(pDblOut->getDims(), pDblOut->getDimsArray());
        for (int i = 0; i < pIOut->getSize(); i++)
        {
            pIOut->set(i, static_cast<int>(pDblOut->get(i)));
        }
        out.push_back(pIOut);
        delete pDblOut;
    }
    else if (in[0]->isInt64())
    {
        types::Int64* pIOut = new types::Int64(pDblOut->getDims(), pDblOut->getDimsArray());
        for (int i = 0; i < pIOut->getSize(); i++)
        {
            pIOut->set(i, static_cast<long long int>(pDblOut->get(i)));
        }
        out.push_back(pIOut);
        delete pDblOut;
    }
    else if (in[0]->isUInt8())
    {
        types::UInt8* pIOut = new types::UInt8(pDblOut->getDims(), pDblOut->getDimsArray());
        for (int i = 0; i < pIOut->getSize(); i++)
        {
            pIOut->set(i, static_cast<unsigned char>(pDblOut->get(i)));
        }
        out.push_back(pIOut);
        delete pDblOut;
    }
    else if (in[0]->isUInt16())
    {
        types::UInt16* pIOut = new types::UInt16(pDblOut->getDims(), pDblOut->getDimsArray());
        for (int i = 0; i < pIOut->getSize(); i++)
        {
            pIOut->set(i, static_cast<unsigned short int>(pDblOut->get(i)));
        }
        out.push_back(pIOut);
        delete pDblOut;
    }
    else if (in[0]->isUInt32())
    {
        types::UInt32* pIOut = new types::UInt32(pDblOut->getDims(), pDblOut->getDimsArray());
        for (int i = 0; i < pIOut->getSize(); i++)
        {
            pIOut->set(i, static_cast<unsigned int>(pDblOut->get(i)));
        }
        out.push_back(pIOut);
        delete pDblOut;
    }
    else if (in[0]->isUInt64())
    {
        types::UInt64* pIOut = new types::UInt64(pDblOut->getDims(), pDblOut->getDimsArray());
        for (int i = 0; i < pIOut->getSize(); i++)
        {
            pIOut->set(i, static_cast<unsigned long long int>(pDblOut->get(i)));
        }
        out.push_back(pIOut);
        delete pDblOut;
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
