/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include "linear_algebra_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"
#include "configvariable.hxx"
#include "callable.hxx"

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
    if(in.size() < 1 && in.size() > 3)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"schur", 1, 3);
        return types::Function::Error;
    }

// *** check type of input args and get it. ***
    if(in[0]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_schur";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    pDbl[0] = in[0]->getAs<types::Double>();

    if(pDbl[0]->getRows() != pDbl[0]->getCols()) // square matrix
    {
        ScierrorW(20, _W("%ls: Wrong type for argument %d: Square matrix expected.\n"), L"schur", 1);
        return types::Function::Error;
    }

    if((pDbl[0]->getRows() == -1) || (pDbl[0]->getCols() == -1)) // manage eye case
    {
        ScierrorW(271,_W("%ls: Size varying argument a*eye(), (arg %d) not allowed here.\n"), L"schur", 1);
        return types::Function::Error;
    }

    pDbl[0] = pDbl[0]->clone()->getAs<types::Double>(); // pDbl will be modified

    if(in.size() > 1)
    {
        if(in[1]->isDouble())
        {
            pDbl[1] = in[1]->getAs<types::Double>();

            if(pDbl[1]->getRows() != pDbl[1]->getCols()) // square matrix
            {
                ScierrorW(20, _W("%ls: Wrong type for argument %d: Square matrix expected.\n"), L"schur", 2);
                return types::Function::Error;
            }

            if((pDbl[1]->getRows() == -1) || (pDbl[1]->getCols() == -1)) // manage eye case
            {
                ScierrorW(271,_W("%ls: Size varying argument a*eye(), (arg %d) not allowed here.\n"), L"schur", 2);
                return types::Function::Error;
            }

            if(pDbl[0]->getCols() != pDbl[1]->getCols())
            {
                ScierrorW(267,_W("%ls: Arg %d and arg %d must have equal dimensions.\n"), L"schur", 1, 2);
                return types::Function::Error;
            }

            pDbl[1] = pDbl[1]->clone()->getAs<types::Double>(); // pDbl will be modified

            iCase = 11;
        }
        else if(in[1]->isString())
        {
            pStr = in[1]->getAs<types::String>();
            bIsRealStr = !wcscmp(pStr->get(0), L"r") || !wcscmp(pStr->get(0), L"real");
            bIsComplexStr = !wcscmp(pStr->get(0), L"comp") || !wcscmp(pStr->get(0), L"complex");

            if(bIsComplexStr)
            {
                pDbl[0]->setComplex(true); // pDbl[0] is a clone of in[0]
            }

            if(bIsRealStr || bIsComplexStr)
            {
                iCase = 1;
            }
            else
            {
                iCase = 12;
            }
        }
        else if(in[1]->isCallable())
        {
            pFunction = in[1]->getAs<types::Callable>();
            iCase = 12;
        }
        else
        {
            std::wstring wstFuncName = L"%"  + in[1]->getShortTypeStr() + L"_schur";
            return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
        }
    }

    if(in.size() == 3)
    {
        if(in[2]->isString() == false && in[2]->isCallable() == false)
        {
            std::wstring wstFuncName = L"%"  + in[2]->getShortTypeStr() + L"_schur";
            return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
        }

        if(in[2]->isString())
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
    switch(iCase)
    {
        case 1: // double
        {
            if(_iRetCount > 2)
            {
                ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d to %d expected.\n"), L"schur", 1, 2);
                return types::Function::Error;
            }
            break;
        }
        case 11: // double double
        {
            if(_iRetCount != 2 && _iRetCount != 4)
            {
                ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d or %d expected.\n"), L"schur", 2, 4);
                return types::Function::Error;
            }
            break;
        }
        case 12: // double string
        {
            if(_iRetCount < 2 && _iRetCount > 3)
            {
                ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d to %d expected.\n"), L"schur", 2, 3);
                return types::Function::Error;
            }
            break;
        }
        default://case 112: // double double string
        {
            if(_iRetCount > 4) // in doc, 5 output args are possible ?!?
            {
                ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d to %d expected.\n"), L"schur", 1, 4);
                return types::Function::Error;
            }
            break;
        }
    }

// *** check empty matrix ***

    // iCase represents the type of args input.
    // 1 = double, 2 = string, so 112 = double double string.
    switch(iCase)
    {
        case 11: // double double
        case 1: // double
        {
            if(pDbl[0]->getCols() == 0)
            {
                for(int i=0; i<_iRetCount; i++)
                {
                    out.push_back(types::Double::Empty());
                }
                return types::Function::OK;
            }
            break;
        }
        case 12: // double string
        {
            if(pDbl[0]->getCols() == 0)
            {
                types::Double* zero = new types::Double(0);

                for(int i = 0; i < _iRetCount; i++)
                {
                    if(i == 1 && !bIsComplexStr && !bIsRealStr)
                    {
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
            if(pDbl[0]->getCols() == 0)
            {
                types::Double* zero = new types::Double(0);

                for(int i=1; i<_iRetCount; i++)
                {
                    out.push_back(types::Double::Empty());
                }

                if(_iRetCount > 1)
                {
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

    if(bIsRealStr && bComplexArgs)
    {
        ScierrorW(202,_W("%ls: Wrong type for input argument #%d: Real matrix expected.\n"), L"schur", 1);
        return types::Function::Error;
    }

    for(int i = 0; i < 2; i++)
    {
        pDblOut[i] = new types::Double(pDbl[0]->getCols(), pDbl[0]->getCols(), bComplexArgs);
    }

    // iCase represents the type of args input.
    // 1 = double, 2 = string, so 112 = double double string.
    switch(iCase)
    {
        case 112: // double double string || double double function
        case 12: // double string || double function
        {
            if(pStr)
            {
                bIsContinuStr = !wcscmp(pStr->get(0), L"c") ||
                                !wcscmp(pStr->get(0), L"cont") ||
                                !wcscmp(pStr->get(0), L"zb02ow") || // two matrix, complex case
                                !wcscmp(pStr->get(0), L"zb02mv") || // one matrix, complex case
                                !wcscmp(pStr->get(0), L"sb02ow") || // two matrix, real case
                                !wcscmp(pStr->get(0), L"sb02mv");   // one matrix, real case

                bIsDiscreteStr = !wcscmp(pStr->get(0), L"d") ||
                                !wcscmp(pStr->get(0), L"disc") ||
                                !wcscmp(pStr->get(0), L"zb02ox") || // two matrix, complex case
                                !wcscmp(pStr->get(0), L"zb02mw") || // one matrix, complex case
                                !wcscmp(pStr->get(0), L"sb02ox") || // two matrix, real case
                                !wcscmp(pStr->get(0), L"sb02mw");   // one matrix, real case

                if(bIsContinuStr == false && bIsDiscreteStr == false)
                {
                    pStrFunction = ConfigVariable::getEntryPoint(pStr->get(0));
                    if(pStrFunction == NULL)
                    {
                        ScierrorW(999,_W("%ls: Subroutine not found: %ls\n"), L"schur", pStr->get(0));
                        return types::Function::Error;
                    }
                }
            }
            else if(pFunction)
            {
                ConfigVariable::setSchurFunction(pFunction);
            }

            pDblOut[2] = new types::Double(1,1); // Dim
            break;
        }
        default:// case 1 and 11
            break;
    }

    int iRet = schurSelect(pDbl, pDblOut, bComplexArgs, bIsDiscreteStr, bIsContinuStr, pStrFunction);

    if(iRet)
    {
        ScierrorW(999,_W("%ls: Schur exit with state %d\n"), L"schur", iRet);
        return types::Function::Error;
    }

    // return result(s)
    switch(iCase)
    {
        case 1: // double
        {
            if(_iRetCount == 2)
            {
                out.push_back(pDblOut[0]);
            }

            out.push_back(pDbl[0]); // pDbl[0] has been overwritten by its real Schur form T.
            break;
        }
        case 11: // double double
        {
            for(int i = 0; i < 2; i++)
            {
                out.push_back(pDbl[i]);
            }

            if(_iRetCount == 4)
            {
                out.push_back(pDblOut[0]);
                if(_iRetCount > 1)
                {
                    out.push_back(pDblOut[1]);
                }
            }

            break;
        }
        case 12: // double string || double function
        {
            if(_iRetCount < 2)
            {
                out.push_back(pDbl[0]);
            }
            else
            {
                out.push_back(pDblOut[0]);
                out.push_back(pDblOut[2]);

                if(_iRetCount == 3)
                {
                    out.push_back(pDbl[0]);
                }
            }
            break;
        }
        case 112: // double double string || double double function
        {
            switch(_iRetCount)
            {
                case 0 :
                case 1 : break; // dim
                case 4 : // As Es Z dim
                {
                    for(int i = 0; i < 2; i++)
                    {
                        out.push_back(pDbl[i]);
                    }
                    out.push_back(pDblOut[1]);
                    break;
                }
                case 3 : // Q Z dim
                {
                    out.push_back(pDblOut[0]);
                }
                case 2 : // Z dim
                {
                    out.push_back(pDblOut[1]);
                    break;
                }
            }
            out.push_back(pDblOut[2]);
            break;
        }
    }

    ConfigVariable::setSchurFunction(NULL);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
