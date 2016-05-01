/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Cedric DELAMARRE
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
#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "overload.hxx"
#include "prod.hxx"
#include "int.hxx"
#include "polynom.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "basic_functions.h"
}


/*
clear a;nb = 2500;a = rand(nb, nb);tic();prod(a);toc
clear a;nb = 2500;a = rand(nb, nb);a = a + a *%i;tic();prod(a);toc
*/

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_prod(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblIn       = NULL;
    types::Double* pDblOut      = NULL;
    types::Polynom* pPolyIn     = NULL;
    types::Polynom* pPolyOut    = NULL;

    int iOrientation    = 0;
    int iOuttype        = 1; // 1 = native | 2 = double (type of output value)

    int* piDimsArray = NULL;

    if (in.size() < 1 || in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "prod", 1, 3);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "prod", 1);
        return types::Function::Error;
    }

    bool isCopy = true;
    /***** get data *****/
    switch (in[0]->getType())
    {
        case types::InternalType::ScilabDouble:
        {
            pDblIn = in[0]->getAs<types::Double>();
            isCopy = false;
            break;
        }
        case types::InternalType::ScilabBool:
        {
            pDblIn = getAsDouble(in[0]->getAs<types::Bool>());
            iOuttype = 2;
            break;
        }
        case types::InternalType::ScilabPolynom:
        {
            pPolyIn = in[0]->getAs<types::Polynom>();
            break;
        }
        case types::InternalType::ScilabInt8:
        {
            pDblIn = getAsDouble(in[0]->getAs<types::Int8>());
            break;
        }
        case types::InternalType::ScilabInt16:
        {
            pDblIn = getAsDouble(in[0]->getAs<types::Int16>());
            break;
        }
        case types::InternalType::ScilabInt32:
        {
            pDblIn = getAsDouble(in[0]->getAs<types::Int32>());
            break;
        }
        case types::InternalType::ScilabInt64:
        {
            pDblIn = getAsDouble(in[0]->getAs<types::Int64>());
            break;
        }
        case types::InternalType::ScilabUInt8:
        {
            pDblIn = getAsDouble(in[0]->getAs<types::UInt8>());
            break;
        }
        case types::InternalType::ScilabUInt16:
        {
            pDblIn = getAsDouble(in[0]->getAs<types::UInt16>());
            break;
        }
        case types::InternalType::ScilabUInt32:
        {
            pDblIn = getAsDouble(in[0]->getAs<types::UInt32>());
            break;
        }
        case types::InternalType::ScilabUInt64:
        {
            pDblIn = getAsDouble(in[0]->getAs<types::UInt64>());
            break;
        }
        default:
        {
            std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_prod";
            return Overload::call(wstFuncName, in, _iRetCount, out);
        }
    }

    if (in.size() >= 2)
    {
        if (in[1]->isDouble())
        {
            types::Double* pDbl = in[1]->getAs<types::Double>();

            if (pDbl->isScalar() == false)
            {
                if (isCopy && pDblIn)
                {
                    pDblIn->killMe();
                }

                Scierror(999, _("%s: Wrong value for input argument #%d: A positive scalar expected.\n"), "prod", 2);
                return types::Function::Error;
            }

            iOrientation = static_cast<int>(pDbl->get(0));

            if (iOrientation <= 0)
            {
                if (isCopy && pDblIn)
                {
                    pDblIn->killMe();
                }

                Scierror(999, _("%s: Wrong value for input argument #%d: A positive scalar expected.\n"), "prod", 2);
                return types::Function::Error;
            }
        }
        else if (in[1]->isString())
        {
            types::String* pStr = in[1]->getAs<types::String>();

            if (pStr->isScalar() == false)
            {
                if (isCopy && pDblIn)
                {
                    pDblIn->killMe();
                }

                Scierror(999, _("%s: Wrong size for input argument #%d: A scalar string expected.\n"), "prod", 2);
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
                const char* pstrExpected = NULL;
                if (in.size() == 2)
                {
                    pstrExpected = "\"*\",\"r\",\"c\",\"m\",\"native\",\"double\"";
                }
                else
                {
                    pstrExpected = "\"*\",\"r\",\"c\",\"m\"";
                }

                if (isCopy && pDblIn)
                {
                    pDblIn->killMe();
                }

                Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "prod", 2, pstrExpected);
                return types::Function::Error;
            }
        }
        else
        {
            if (isCopy && pDblIn)
            {
                pDblIn->killMe();
            }

            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix or a string expected.\n"), "prod", 2);
            return types::Function::Error;
        }
    }

    if (in.size() == 3)
    {
        if (in[2]->isString() == false)
        {
            if (isCopy && pDblIn)
            {
                pDblIn->killMe();
            }

            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "prod", 3);
            return types::Function::Error;
        }

        types::String* pStr = in[2]->getAs<types::String>();

        if (pStr->isScalar() == false)
        {
            if (isCopy && pDblIn)
            {
                pDblIn->killMe();
            }

            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar string expected.\n"), "prod", 3);
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
            if (isCopy && pDblIn)
            {
                pDblIn->killMe();
            }

            Scierror(999, _("%s: Wrong value for input argument #%d: %s or %s expected.\n"), "prod", 3, "\"native\"", "\"double\"");
            return types::Function::Error;
        }
    }

    /***** perform operation *****/
    if (pDblIn)
    {
        if (pDblIn->isEmpty())
        {
            if (iOrientation == 0)
            {
                pDblOut = new types::Double(1);
                out.push_back(pDblOut);
            }
            else
            {
                out.push_back(types::Double::Empty());
            }

            if (isCopy)
            {
                delete pDblIn;
                pDblIn = NULL;
            }

            return types::Function::OK;
        }

        if (iOrientation > pDblIn->getDims())
        {
            if (isCopy)
            {
                pDblOut = pDblIn;
            }
            else
            {
                pDblOut = pDblIn->clone()->getAs<types::Double>();
            }
        }
        else
        {
            pDblOut = prod(pDblIn, iOrientation);
            if (isCopy)
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
            pPolyOut = prod(pPolyIn, iOrientation);
        }
    }

    /***** set result *****/
    if ((iOuttype == 1) && isCopy)
    {
        switch (in[0]->getType())
        {
            case types::InternalType::ScilabBool:
            {
                types::Bool* pB = new types::Bool(pDblOut->getDims(), pDblOut->getDimsArray());
                int* p = pB->get();
                double* pd = pDblOut->get();
                int size = pB->getSize();
                for (int i = 0; i < size; ++i)
                {
                    p[i] = pd[i] != 0 ? 1 : 0;
                }
                out.push_back(pB);
                break;
            }
            case types::InternalType::ScilabPolynom:
            {
                out.push_back(pPolyOut);
                break;
            }
            case types::InternalType::ScilabInt8:
            {
                out.push_back(toInt<types::Int8>(pDblOut));
                break;
            }
            case types::InternalType::ScilabInt16:
            {
                out.push_back(toInt<types::Int16>(pDblOut));
                break;
            }
            case types::InternalType::ScilabInt32:
            {
                out.push_back(toInt<types::Int32>(pDblOut));
                break;
            }
            case types::InternalType::ScilabInt64:
            {
                out.push_back(toInt<types::Int64>(pDblOut));
                break;
            }
            case types::InternalType::ScilabUInt8:
            {
                out.push_back(toInt<types::UInt8>(pDblOut));
                break;
            }
            case types::InternalType::ScilabUInt16:
            {
                out.push_back(toInt<types::UInt16>(pDblOut));
                break;
            }
            case types::InternalType::ScilabUInt32:
            {
                out.push_back(toInt<types::UInt32>(pDblOut));
                break;
            }
            case types::InternalType::ScilabUInt64:
            {
                out.push_back(toInt<types::UInt64>(pDblOut));
                break;
            }
            default:
                return types::Function::Error;
        }

        if (pDblOut)
        {
            pDblOut->killMe();
        }
    }
    else
    {
        out.push_back(pDblOut);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
