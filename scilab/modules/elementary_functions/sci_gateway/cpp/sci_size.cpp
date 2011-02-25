/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Antoine ELIAS
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
#include "funcmanager.hxx"
#include "context.hxx"
#include "types.hxx"
#include "string.hxx"
#include "container.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

int getProcessMode(types::typed_list &in, int _iProcess, int _iRef);

/*--------------------------------------------------------------------------*/
using namespace types;

Function::ReturnValue sci_size(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if(in.size() < 1)
    {
        Scierror(999,_("%s: Wrong number of input arguments: At least %d expected.\n"), "size", 1);
        return Function::Error;
    }

    switch(in[0]->getType())
    {
        // Dedicated case for lists.
    case InternalType::RealList:
    case InternalType::RealMList:
    case InternalType::RealStruct:
    case InternalType::RealTList:
        {
            if(in.size() > 1)
            {
                Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "size", 1);
                return Function::Error;
            }

            Double* pD = new Double(in[0]->getAsContainer()->getSize());
            out.push_back(pD);
            break;
        }
    default :
        // All types inherits of GenericType, so have this algorithm as default.
        {
            int iMode = -1;

            if(in.size() > 2)
            {
                Scierror(77,_("%s: Wrong number of input argument(s): %d to %d expected.\n"), "size", 1, 2);
                return Function::Error;
            }

            if(in.size() == 2)
            {
                iMode = getProcessMode(in, 1, 0);
                if((iMode == 1 || iMode == 2) && in[0]->getAs<Double>()->getDims() > 2)
                {
                    ScierrorW(999, _W("%ls: Wrong values for input argument #%d: '%ls' or '%ls' expected.\n"), L"size", 2, L"*", L"0");
                    return Function::Error;
                }
            }

            if(iMode == -2)
            {
                return Function::Error;
            }

            int iDims   = in[0]->getAsGenericType()->getDims();
            int* piDims = in[0]->getAsGenericType()->getDimsArray();

            if(_iRetCount == 1)
            {
                int iRowsOut = 1;
                int iColsOut = 0;
                double* pdblReal = NULL;

                switch(iMode)
                {
                case -1 : //lhs == 1
                    iColsOut = iDims;
                    //pdblReal[0] = iRows;
                    //pdblReal[1] = iCols;
                    break;
                case 0 : //"*"
                    iColsOut = 1;
                    //pdblReal[0] = iRows * iCols;
                    break;
                case 1 : //"r"
                    iColsOut = 1;
                    //pdblReal[0] = iRows;
                    break;
                case 2 : //"c"
                    iColsOut = 1;
                    //pdblReal[0] = iCols;
                    break;
                }

                Double* pD = new Double(iRowsOut, iColsOut);

                double* pdbl = pD->getReal();

                switch(iMode)
                {
                case -1 : //lhs == 1
                    for(int i = 0 ; i < iDims ; i++)
                    {
                        pdbl[i] = piDims[i];
                    }
                    break;
                case 0 : //"*"
                    pdbl[0] = in[0]->getAsGenericType()->getSize();
                    break;
                case 1 : //"r"
                    iColsOut = 1;
                    pdbl[0] = in[0]->getAsGenericType()->getRows();
                    break;
                case 2 : //"c"
                    iColsOut = 1;
                    pdbl[0] = in[0]->getAsGenericType()->getCols();
                    break;
                }
                out.push_back(pD);
            }
            else
            {
                for(int i = 0 ; i < Min(_iRetCount, iDims) ; i++)
                {
                    Double* pD = new Double(piDims[i]);
                    out.push_back(pD);
                }
            }
            break;
        }
    }
    return Function::OK;
}

/*return process mode, "r", "c", "*", 0, 1, 2*/
int getProcessMode(types::typed_list &in, int _iProcess, int _iRef)
{
    int iMode = 0;
    if(in[_iProcess]->isString())
    {
        String* pS = in[_iProcess]->getAs<types::String>();
        if(pS->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for argument %d: (%d,%d) expected.\n"), "size", _iProcess + 1, 1, 1);
        }

        switch(pS->get(0)[0])
        {
        case 'r' :
            iMode = 1;
            break;
        case 'c' :
            iMode = 2;
            break;
        case '*' :
            iMode = 0;
            break;
        case 'm' :
            iMode = -1;
            break;
        default :
            Scierror(999,_("%s: Wrong value for input argument #%d: '%s', '%s', '%s' or '%s' expected.\n"), "size", _iProcess + 1, "m" , "*" , "r", "c");
            iMode = -2;
            break;
        }
    }
    else if(in[1]->isDouble() && in[1]->getAs<Double>()->isComplex() == false)
    {
        Double* pD = in[_iProcess]->getAs<Double>();
        if(pD->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for argument %d: (%d,%d) expected.\n"), "size", _iProcess + 1, 1, 1);
            iMode = -2;
        }

        iMode = static_cast<int>(pD->getReal()[0]);
        if(iMode != -1 && iMode != 0 && iMode != 1 && iMode != 2)
        {
            Scierror(999,_("%s: Wrong value for input argument #%d: '%s', '%s', '%s' or '%s' expected.\n"), "size", _iProcess + 1, "-1" , "0" , "1", "2");
            iMode = -2;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string or a scalar expected.\n"), "size", 2);
        iMode = -2;
    }

    //special case for -1
    if(iMode == -1)
    {
        iMode = 0;
        if(in[_iRef]->getAsGenericType()->getRows() > 1)
        {
            iMode = 1;
        }
        else if(in[_iRef]->getAsGenericType()->getCols() > 1)
        {
            iMode = 2;
        }
    }

    return iMode;
}
/*--------------------------------------------------------------------------*/
