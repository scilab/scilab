/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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

#include "linear_algebra_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"
#include "configvariable.hxx"
#include "callable.hxx"
#include "string.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "scischur.h"
#include "schurSelect.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_schur(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDbl[2]          = {NULL, NULL};
    types::String* pStr             = NULL;
    types::Double* pDblOut[3]       = {NULL, NULL, NULL};
    types::Callable* pFunction      = NULL;
    ConfigVariable::EntryPointStr* pStrFunction = NULL;

    int  iCase                      = 1;
    bool bComplexArgs               = false;
    bool bIsRealStr                 = false;
    bool bIsComplexStr              = false;
    bool bIsContinuStr              = false;
    bool bIsDiscreteStr             = false;

    // *** check number of input args. ***
    if (in.size() < 1 || in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "schur", 1, 3);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    if (in[0]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_schur";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    pDbl[0] = in[0]->getAs<types::Double>();

    if (pDbl[0]->getRows() != pDbl[0]->getCols()) // square matrix
    {
        Scierror(20, _("%s: Wrong type for argument %d: Square matrix expected.\n"), "schur", 1);
        return types::Function::Error;
    }

    if ((pDbl[0]->getRows() == -1) || (pDbl[0]->getCols() == -1)) // manage eye case
    {
        Scierror(271, _("%s: Size varying argument a*eye(), (arg %d) not allowed here.\n"), "schur", 1);
        return types::Function::Error;
    }

    pDbl[0] = pDbl[0]->clone()->getAs<types::Double>(); // pDbl will be modified

    if (in.size() > 1)
    {
        if (in[1]->isDouble())
        {
            pDbl[1] = in[1]->getAs<types::Double>();

            if (pDbl[1]->getRows() != pDbl[1]->getCols()) // square matrix
            {
                Scierror(20, _("%s: Wrong type for argument %d: Square matrix expected.\n"), "schur", 2);
                return types::Function::Error;
            }

            if ((pDbl[1]->getRows() == -1) || (pDbl[1]->getCols() == -1)) // manage eye case
            {
                Scierror(271, _("%s: Size varying argument a*eye(), (arg %d) not allowed here.\n"), "schur", 2);
                return types::Function::Error;
            }

            if (pDbl[0]->getCols() != pDbl[1]->getCols())
            {
                Scierror(267, _("%s: Arg %d and arg %d must have equal dimensions.\n"), "schur", 1, 2);
                return types::Function::Error;
            }

            pDbl[1] = pDbl[1]->clone()->getAs<types::Double>(); // pDbl will be modified

            iCase = 11;
        }
        else if (in[1]->isString())
        {
            pStr = in[1]->getAs<types::String>();
            bIsRealStr = !wcscmp(pStr->get(0), L"r") || !wcscmp(pStr->get(0), L"real");
            bIsComplexStr = !wcscmp(pStr->get(0), L"comp") || !wcscmp(pStr->get(0), L"complex");

            if (bIsComplexStr)
            {
                pDbl[0]->setComplex(true); // pDbl[0] is a clone of in[0]
            }

            if (bIsRealStr || bIsComplexStr)
            {
                iCase = 1;
            }
            else
            {
                iCase = 12;
            }
        }
        else if (in[1]->isCallable())
        {
            pFunction = in[1]->getAs<types::Callable>();
            iCase = 12;
        }
        else
        {
            std::wstring wstFuncName = L"%" + in[1]->getShortTypeStr() + L"_schur";
            return Overload::call(wstFuncName, in, _iRetCount, out);
        }
    }

    if (in.size() == 3)
    {
        if (in[2]->isString() == false && in[2]->isCallable() == false)
        {
            std::wstring wstFuncName = L"%" + in[2]->getShortTypeStr() + L"_schur";
            return Overload::call(wstFuncName, in, _iRetCount, out);
        }

        if (in[2]->isString())
        {
            pStr = in[2]->getAs<types::String>();
        }
        else
        {
            pFunction = in[2]->getAs<types::Callable>();
        }

        iCase = 112;
    }

    // *** check number of output args according the input args. ***

    // iCase represents the type of args input.
    // 1 = double, 2 = string, so 112 = double double string.
    switch (iCase)
    {
        case 1: // double
        {
            if (_iRetCount > 2)
            {
                Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "schur", 1, 2);
                return types::Function::Error;
            }
            break;
        }
        case 11: // double double
        {
            if (_iRetCount != 2 && _iRetCount != 4)
            {
                Scierror(78, _("%s: Wrong number of output argument(s): %d or %d expected.\n"), "schur", 2, 4);
                return types::Function::Error;
            }
            break;
        }
        case 12: // double string
        {
            if (_iRetCount < 2 && _iRetCount > 3)
            {
                Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "schur", 2, 3);
                return types::Function::Error;
            }
            break;
        }
        default://case 112: // double double string
        {
            if (_iRetCount > 4) // in doc, 5 output args are possible ?!?
            {
                Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "schur", 1, 4);
                return types::Function::Error;
            }
            break;
        }
    }

    // *** check empty matrix ***

    // iCase represents the type of args input.
    // 1 = double, 2 = string, so 112 = double double string.
    switch (iCase)
    {
        case 11: // double double
        case 1: // double
        {
            if (pDbl[0]->getCols() == 0)
            {
                for (int i = 0; i < _iRetCount; i++)
                {
                    out.push_back(types::Double::Empty());
                }
                return types::Function::OK;
            }
            break;
        }
        case 12: // double string
        {
            if (pDbl[0]->getCols() == 0)
            {
                for (int i = 0; i < _iRetCount; i++)
                {
                    if (i == 1 && !bIsComplexStr && !bIsRealStr)
                    {
                        types::Double* zero = new types::Double(0);
                        out.push_back(zero);
                    }
                    else
                    {
                        out.push_back(types::Double::Empty());
                    }
                }
                return types::Function::OK;
            }
            break;
        }
        case 112: // double double string
        {
            if (pDbl[0]->getCols() == 0)
            {
                for (int i = 1; i < _iRetCount; i++)
                {
                    out.push_back(types::Double::Empty());
                }

                if (_iRetCount > 1)
                {
                    types::Double* zero = new types::Double(0);
                    out.push_back(zero);
                }
                else
                {
                    out.push_back(types::Double::Empty());
                }

                return types::Function::OK;
            }
            break;
        }
    }

    // *** perform operations. ***

    bComplexArgs = pDbl[0]->isComplex() || (pDbl[1] && pDbl[1]->isComplex()) || bIsComplexStr;

    if (bIsRealStr && bComplexArgs)
    {
        Scierror(202, _("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "schur", 1);
        return types::Function::Error;
    }

    for (int i = 0; i < 2; i++)
    {
        pDblOut[i] = new types::Double(pDbl[0]->getCols(), pDbl[0]->getCols(), bComplexArgs);
    }

    // iCase represents the type of args input.
    // 1 = double, 2 = string, so 112 = double double string.
    switch (iCase)
    {
        case 112: // double double string || double double function
        case 12: // double string || double function
        {
            if (pStr)
            {
                wchar_t* pst = pStr->get(0);
                bIsContinuStr = !wcscmp(pst, L"c") ||
                                !wcscmp(pst, L"cont") ||
                                !wcscmp(pst, L"zb02ow") || // two matrix, complex case
                                !wcscmp(pst, L"zb02mv") || // one matrix, complex case
                                !wcscmp(pst, L"sb02ow") || // two matrix, real case
                                !wcscmp(pst, L"sb02mv");   // one matrix, real case

                bIsDiscreteStr = !wcscmp(pst, L"d") ||
                                 !wcscmp(pst, L"disc") ||
                                 !wcscmp(pst, L"zb02ox") || // two matrix, complex case
                                 !wcscmp(pst, L"zb02mw") || // one matrix, complex case
                                 !wcscmp(pst, L"sb02ox") || // two matrix, real case
                                 !wcscmp(pst, L"sb02mw");   // one matrix, real case

                if (bIsContinuStr == false && bIsDiscreteStr == false)
                {
                    pStrFunction = ConfigVariable::getEntryPoint(pStr->get(0));
                    if (pStrFunction == NULL)
                    {
                        char* pst = wide_string_to_UTF8(pStr->get(0));
                        Scierror(999, _("%s: Subroutine not found: %s\n"), "schur", pst);
                        FREE(pst);
                        return types::Function::Error;
                    }
                }
            }
            else if (pFunction)
            {
                ConfigVariable::setSchurFunction(pFunction);
            }

            pDblOut[2] = new types::Double(1, 1); // Dim
            break;
        }
        default:// case 1 and 11
            break;
    }

    int iRet = schurSelect(pDbl, pDblOut, bComplexArgs, bIsDiscreteStr, bIsContinuStr, pStrFunction);

    if (iRet)
    {
        Scierror(999, _("%s: Schur exit with state %d\n"), "schur", iRet);
        return types::Function::Error;
    }

    // return result(s)
    switch (iCase)
    {
        case 1: // double
        {
            if (_iRetCount == 2)
            {
                out.push_back(pDblOut[0]);
                pDblOut[0] = NULL;
            }

            out.push_back(pDbl[0]); // pDbl[0] has been overwritten by its real Schur form T.

            break;
        }
        case 11: // double double
        {
            for (int i = 0; i < 2; i++)
            {
                out.push_back(pDbl[i]);
                pDbl[i] = NULL;
            }

            if (_iRetCount == 4)
            {
                out.push_back(pDblOut[0]);
                pDblOut[0] = NULL;
                if (_iRetCount > 1)
                {
                    out.push_back(pDblOut[1]);
                    pDblOut[1] = NULL;
                }
            }

            break;
        }
        case 12: // double string || double function
        {
            if (_iRetCount < 2)
            {
                out.push_back(pDbl[0]);
            }
            else
            {
                out.push_back(pDblOut[0]);
                pDblOut[0] = NULL;
                out.push_back(pDblOut[2]);
                pDblOut[2] = NULL;

                if (_iRetCount == 3)
                {
                    out.push_back(pDbl[0]);
                }
            }
            break;
        }
        case 112: // double double string || double double function
        {
            switch (_iRetCount)
            {
                case 0 :
                case 1 :
                    break; // dim
                case 4 : // As Es Z dim
                {
                    for (int i = 0; i < 2; i++)
                    {
                        out.push_back(pDbl[i]);
                        pDbl[i] = NULL;
                    }
                    out.push_back(pDblOut[1]);
                    pDblOut[1] = NULL;
                    break;
                }
                case 3 : // Q Z dim
                {
                    out.push_back(pDblOut[0]);
                    pDblOut[0] = NULL;
                }
                case 2 : // Z dim
                {
                    out.push_back(pDblOut[1]);
                    pDblOut[1] = NULL;
                    break;
                }
            }
            out.push_back(pDblOut[2]);
            pDblOut[2] = NULL;
            break;
        }
    }

    for (int i = 0; i < 3; ++i)
    {
        if (pDblOut[i])
        {
            pDblOut[i]->killMe();
        }
    }


    if (pDbl[1])
    {
        pDbl[1]->killMe();
    }
    ConfigVariable::setSchurFunction(NULL);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
