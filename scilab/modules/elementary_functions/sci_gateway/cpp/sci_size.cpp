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
#include "elem_func_gw.hxx"
#include "funcmanager.hxx"
#include "context.hxx"
#include "types.hxx"

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
    case InternalType::RealBool:
    case InternalType::RealCell:
    case InternalType::RealDouble:
    case InternalType::RealFloat:
    case InternalType::RealInt:
    case InternalType::RealPoly:
    case InternalType::RealString:
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
            }

            if(iMode == -2)
            {
                return Function::Error;
            }

            int iRows = in[0]->getAsGenericType()->rows_get();
            int iCols = in[0]->getAsGenericType()->cols_get();

            if(_iRetCount == 1)
            {
                int iRowsOut = 1;
                int iColsOut = 0;
                double pdblReal[2] = {0,0};

                switch(iMode)
                {
                case -1 : //lhs == 1
                    iColsOut = 2;
                    pdblReal[0] = iRows;
                    pdblReal[1] = iCols;
                    break;
                case 0 : //"*"
                    iColsOut = 1;
                    pdblReal[0] = iRows * iCols;
                    break;
                case 1 : //"r"
                    iColsOut = 1;
                    pdblReal[0] = iRows;
                    break;
                case 2 : //"c"
                    iColsOut = 1;
                    pdblReal[0] = iCols;
                    break;
                }
                Double* pD = new Double(iRowsOut, iColsOut);
                pD->real_get()[0] = pdblReal[0];
                if(iColsOut == 2)
                {
                    pD->real_get()[1] = pdblReal[1];
                }
                out.push_back(pD);
            }
            else
            {
                Double* pD1 = new Double(iRows);
                Double* pD2 = new Double(iCols);
                out.push_back(pD1);
                out.push_back(pD2);
            }
            break;
        }

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

            Double* pD = new Double(in[0]->getAsContainer()->size_get());
            out.push_back(pD);
            break;
        }
    }
    return Function::OK;
}

/*return process mode, "r", "c", "*", 0, 1, 2*/
int getProcessMode(types::typed_list &in, int _iProcess, int _iRef)
{
    int iMode = 0;
    if(in[_iProcess]->getType() == InternalType::RealString)
    {
        String* pS = in[_iProcess]->getAsString();
        if(pS->size_get() != 1)
        {
            Scierror(999, _("%s: Wrong size for argument %d: (%d,%d) expected.\n"), "size", _iProcess + 1, 1, 1);
        }

        switch(pS->string_get(0)[0])
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
    else if(in[1]->getType() == InternalType::RealDouble && in[1]->getAsDouble()->isComplex() == false)
    {
        Double* pD = in[_iProcess]->getAsDouble();
        if(pD->size_get() != 1)
        {
            Scierror(999, _("%s: Wrong size for argument %d: (%d,%d) expected.\n"), "size", _iProcess + 1, 1, 1);
            iMode = -2;
        }

        iMode = pD->real_get()[0];
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
        if(in[_iRef]->getAsGenericType()->rows_get() > 1)
        {
            iMode = 1;
        }
        else if(in[_iRef]->getAsGenericType()->cols_get() > 1)
        {
            iMode = 2;
        }
    }

    return iMode;
}
/*--------------------------------------------------------------------------*/
