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
#include "string.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"
#include "cumsum.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "basic_functions.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_cumsum(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblIn       = NULL;
    types::Double* pDblOut      = NULL;
    types::Polynom* pPolyIn     = NULL;
    types::Polynom* pPolyOut    = NULL;

    int iOrientation    = 0;
    int iOuttype        = 1; // 1 = native | 2 = double (type of output value)

    if (in.size() < 1 || in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "cumsum", 1, 3);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "cumsum", 1);
        return types::Function::Error;
    }

    /***** get data *****/
    if (in[0]->isDouble()) // double
    {
        pDblIn = in[0]->getAs<types::Double>();
    }
    else if (in[0]->isBool()) // bool
    {
        types::Bool* pBIn = in[0]->getAs<types::Bool>();
        pDblIn = new types::Double(pBIn->getDims(), pBIn->getDimsArray());
        for (int i = 0; i < pBIn->getSize(); i++)
        {
            pBIn->get(i) ? pDblIn->set(i, 1) : pDblIn->set(i, 0);
        }
        iOuttype = 2;
    }
    else if (in[0]->isPoly()) // polynom
    {
        pPolyIn = in[0]->getAs<types::Polynom>();
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
        types::UInt8* pIIn = in[0]->getAs<types::UInt8>();
        pDblIn = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
        for (int i = 0; i < pIIn->getSize(); i++)
        {
            pDblIn->set(i, static_cast<double>(pIIn->get(i)));
        }
    }
    else if (in[0]->isUInt16())
    {
        types::UInt16* pIIn = in[0]->getAs<types::UInt16>();
        pDblIn = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
        for (int i = 0; i < pIIn->getSize(); i++)
        {
            pDblIn->set(i, static_cast<double>(pIIn->get(i)));
        }
    }
    else if (in[0]->isUInt32())
    {
        types::UInt32* pIIn = in[0]->getAs<types::UInt32>();
        pDblIn = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
        for (int i = 0; i < pIIn->getSize(); i++)
        {
            pDblIn->set(i, static_cast<double>(pIIn->get(i)));
        }
    }
    else if (in[0]->isUInt64())
    {
        types::UInt64* pIIn = in[0]->getAs<types::UInt64>();
        pDblIn = new types::Double(pIIn->getDims(), pIIn->getDimsArray());
        for (int i = 0; i < pIIn->getSize(); i++)
        {
            pDblIn->set(i, static_cast<double>(pIIn->get(i)));
        }
    }
    else
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_cumsum";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    if (in.size() >= 2)
    {
        if (in[1]->isDouble())
        {
            types::Double* pDbl = in[1]->getAs<types::Double>();

            if (pDbl->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: A positive scalar expected.\n"), "cumsum", 2);
                return types::Function::Error;
            }

            iOrientation = static_cast<int>(pDbl->get(0));

            if (iOrientation <= 0)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: A positive scalar expected.\n"), "cumsum", 2);
                return types::Function::Error;
            }
        }
        else if (in[1]->isString())
        {
            types::String* pStr = in[1]->getAs<types::String>();

            if (pStr->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A scalar string expected.\n"), "cumsum", 2);
                return types::Function::Error;
            }

            wchar_t* wcsString = pStr->get(0);

            if (wcscmp(wcsString, L"*") == 0)
            {
                iOrientation = 0;
            }
            else if (wcscmp(wcsString, L"r") == 0)
            {
                iOrientation = 1;
            }
            else if (wcscmp(wcsString, L"c") == 0)
            {
                iOrientation = 2;
            }
            else if (wcscmp(wcsString, L"m") == 0)
            {
                int iDims = 0;
                int* piDimsArray = NULL;

                if (pDblIn)
                {
                    iDims = pDblIn->getDims();
                    piDimsArray = pDblIn->getDimsArray();
                }
                else
                {
                    iDims = pPolyIn->getDims();
                    piDimsArray = pPolyIn->getDimsArray();
                }

                // old function was "mtlsel"
                for (int i = 0; i < iDims; i++)
                {
                    if (piDimsArray[i] > 1)
                    {
                        iOrientation = i + 1;
                        break;
                    }
                }
            }
            else if ((wcscmp(wcsString, L"native") == 0) && (in.size() == 2))
            {
                iOuttype = 1;
            }
            else if ((wcscmp(wcsString, L"double") == 0) && (in.size() == 2))
            {
                iOuttype = 2;
            }
            else
            {
                char* pstrExpected = NULL;
                if (in.size() == 2)
                {
                    pstrExpected = "\"*\",\"r\",\"c\",\"m\",\"native\",\"double\"";
                }
                else
                {
                    pstrExpected = "\"*\",\"r\",\"c\",\"m\"";
                }

                Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "cumsum", 2, pstrExpected);
                return types::Function::Error;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix or a string expected.\n"), "cumsum", 2);
            return types::Function::Error;
        }
    }

    if (in.size() == 3)
    {
        if (in[2]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "cumsum", 3);
            return types::Function::Error;
        }

        types::String* pStr = in[2]->getAs<types::String>();

        if (pStr->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar string expected.\n"), "cumsum", 3);
            return types::Function::Error;
        }

        wchar_t* wcsString = pStr->get(0);

        if (wcscmp(wcsString, L"native") == 0)
        {
            iOuttype = 1;
        }
        else if (wcscmp(wcsString, L"double") == 0)
        {
            iOuttype = 2;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: %s or %s expected.\n"), "cumsum", 3, "\"native\"", "\"double\"");
            return types::Function::Error;
        }
    }

    /***** perform operation *****/
    if (pDblIn)
    {
        if (iOrientation > pDblIn->getDims())
        {
            if (in[0]->isDouble())
            {
                pDblOut = pDblIn->clone()->getAs<types::Double>();
            }
            else
            {
                pDblOut = pDblIn;
            }

            if (in[0]->isBool() == false)
            {
                iOuttype = 2;
            }
        }
        else
        {
            pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), pDblIn->isComplex());
            cumsum(pDblIn, iOrientation, pDblOut);
            if (in[0]->isDouble() == false)
            {
                delete pDblIn;
                pDblIn = NULL;
            }
        }
    }
    else if (pPolyIn)
    {
        iOuttype = 1;
        if (iOrientation > pPolyIn->getDims())
        {
            pPolyOut = pPolyIn->clone()->getAs<types::Polynom>();
        }
        else
        {
            int* piRanks = (int*)malloc(pPolyIn->getSize() * sizeof(int));
            pPolyIn->getRank(piRanks);
            pPolyOut = new types::Polynom(pPolyIn->getVariableName(), pPolyIn->getDims(), pPolyIn->getDimsArray(), piRanks);
            pPolyOut->setComplex(pPolyIn->isComplex());
            cumsum(pPolyIn, iOrientation, pPolyOut);
        }
    }

    /***** set result *****/
    if ((iOuttype == 1) && (in[0]->isDouble() == false))
    {
        if (in[0]->isBool())
        {
            types::Bool* pBOut = new types::Bool(pDblOut->getDims(), pDblOut->getDimsArray());
            for (int i = 0; i < pDblOut->getSize(); i++)
            {
                pDblOut->get(i) ? pBOut->set(i, 1) : pBOut->set(i, 0);
            }
            out.push_back(pBOut);
        }
        else if (in[0]->isPoly())
        {
            out.push_back(pPolyOut);
        }
        else if (in[0]->isInt8())
        {
            types::Int8* pIOut = new types::Int8(pDblOut->getDims(), pDblOut->getDimsArray());
            for (int i = 0; i < pIOut->getSize(); i++)
            {
                pIOut->set(i, static_cast<char>(pDblOut->get(i)));
            }
            out.push_back(pIOut);
        }
        else if (in[0]->isInt16())
        {
            types::Int16* pIOut = new types::Int16(pDblOut->getDims(), pDblOut->getDimsArray());
            for (int i = 0; i < pIOut->getSize(); i++)
            {
                pIOut->set(i, static_cast<short int>(pDblOut->get(i)));
            }
            out.push_back(pIOut);
        }
        else if (in[0]->isInt32())
        {
            types::Int32* pIOut = new types::Int32(pDblOut->getDims(), pDblOut->getDimsArray());
            for (int i = 0; i < pIOut->getSize(); i++)
            {
                pIOut->set(i, static_cast<int>(pDblOut->get(i)));
            }
            out.push_back(pIOut);
        }
        else if (in[0]->isInt64())
        {
            types::Int64* pIOut = new types::Int64(pDblOut->getDims(), pDblOut->getDimsArray());
            for (int i = 0; i < pIOut->getSize(); i++)
            {
                pIOut->set(i, static_cast<long long int>(pDblOut->get(i)));
            }
            out.push_back(pIOut);
        }
        else if (in[0]->isUInt8())
        {
            types::UInt8* pIOut = new types::UInt8(pDblOut->getDims(), pDblOut->getDimsArray());
            for (int i = 0; i < pIOut->getSize(); i++)
            {
                pIOut->set(i, static_cast<unsigned char>(pDblOut->get(i)));
            }
            out.push_back(pIOut);
        }
        else if (in[0]->isUInt16())
        {
            types::UInt16* pIOut = new types::UInt16(pDblOut->getDims(), pDblOut->getDimsArray());
            for (int i = 0; i < pIOut->getSize(); i++)
            {
                pIOut->set(i, static_cast<unsigned short int>(pDblOut->get(i)));
            }
            out.push_back(pIOut);
        }
        else if (in[0]->isUInt32())
        {
            types::UInt32* pIOut = new types::UInt32(pDblOut->getDims(), pDblOut->getDimsArray());
            for (int i = 0; i < pIOut->getSize(); i++)
            {
                pIOut->set(i, static_cast<unsigned int>(pDblOut->get(i)));
            }
            out.push_back(pIOut);
        }
        else if (in[0]->isUInt64())
        {
            types::UInt64* pIOut = new types::UInt64(pDblOut->getDims(), pDblOut->getDimsArray());
            for (int i = 0; i < pIOut->getSize(); i++)
            {
                pIOut->set(i, static_cast<unsigned long long int>(pDblOut->get(i)));
            }
            out.push_back(pIOut);
        }
    }
    else
    {
        out.push_back(pDblOut);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
