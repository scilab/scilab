/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Antoine ELIAS
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
#include "types.hxx"
#include "string.hxx"
#include "container.hxx"
#include "getmode.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"


extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "os_swprintf.h"
}

using namespace types;
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_size(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() < 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: At least %d expected.\n"), "size", 1);
        return Function::Error;
    }

    switch (in[0]->getType())
    {
            // Dedicated case for lists.
        case InternalType::RealMList:
        {
            std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_size";
            Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
            break;
        }
        case InternalType::RealTList:
        {
            // calls the overload if it exists.
            std::wstring wstFuncName = L"%"  + in[0]->getTypeStr() + L"_size";
            types::InternalType *pIT = symbol::Context::getInstance()->get(symbol::Symbol(wstFuncName));
            if(pIT)
            {
                Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
                break;
            }
        }
        case InternalType::RealList:
        {
            if (in.size() > 1)
            {
                Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "size", 1);
                return Function::Error;
            }

            Double* pD = new Double(in[0]->getAs<Container>()->getSize());
            out.push_back(pD);
            break;
        }
        default :
            // All types inherits of GenericType, so have this algorithm as default.
        {
            int iMode = -1;

            if (in.size() > 2)
            {
                Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "size", 1, 2);
                return Function::Error;
            }

            if (in.size() == 2)
            {
                iMode = getMode(in, 1, 0);
                if (iMode == -2)
                {
                    return Function::Error;
                }
            }

            int iDims   = in[0]->getAs<GenericType>()->getDims();
            int* piDims = in[0]->getAs<GenericType>()->getDimsArray();

            if (_iRetCount == 1)
            {
                int iRowsOut = 1;
                int iColsOut = 0;
                double* pdblReal = NULL;

                switch (iMode)
                {
                    case -1 : //lhs == 1
                        iColsOut = iDims;
                        break;
                    default : //"*"
                        iColsOut = 1;
                        break;
                }

                Double* pD = new Double(iRowsOut, iColsOut);

                double* pdbl = pD->getReal();

                switch (iMode)
                {
                    case -1 : //lhs == 1
                        for (int i = 0 ; i < iDims ; i++)
                        {
                            pdbl[i] = piDims[i];
                        }
                        break;
                    case 0 : //"*"
                        pdbl[0] = in[0]->getAs<GenericType>()->getSize();
                        break;
                    default : //"r"
                        if (iMode > iDims)
                        {
                            pdbl[0] = 1;
                            out.push_back(pD);
                            return Function::OK;
                        }

                        iColsOut = 1;
                        pdbl[0] = piDims[iMode - 1];
                        break;
                }
                out.push_back(pD);
            }
            else
            {
                for (int i = 0 ; i < Min(_iRetCount, iDims) ; i++)
                {
                    Double* pD = new Double(piDims[i]);
                    out.push_back(pD);
                }

                /* Multiple returns:
                 * example: [a b c]=size(M) */
                if (_iRetCount > iDims)
                {
                    for (int i = iDims ; i < _iRetCount ; i++)
                    {
                        Double* pD = new Double(1);
                        out.push_back(pD);
                    }
                }
            }
            break;
        }
    }
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
