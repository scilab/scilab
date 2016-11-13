/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
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

#include "core_gw.hxx"
#include "double.hxx"
#include "int.hxx"
#include "cell.hxx"
#include "function.hxx"
#include "funcmanager.hxx"
#include "types_tools.hxx"

extern "C"
{
#include "Scierror.h"
}

template <class T>
int get_dimsarray(T* _pIn, int _iSize, int* _pOut)
{
    int iProd = 1;
    for (int i = 0 ; i < _iSize ; i++)
    {
        _pOut[i] = (int)_pIn[i];
        iProd *= _pOut[i];
    }
    return iProd;
}

types::Function::ReturnValue sci_makecell(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iProd = 0;
    int iDims = 0;
    int* piDimsArray = NULL;

    if (in.size() < 2)
    {
        Scierror(999, _("%s: Wrong size for input arguments: more than %d expected.\n"), "makecell", 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(999, _("%s: Wrong size for output arguments: %d expected.\n"), "makecell", 1);
        return types::Function::Error;
    }

    //check data type
    switch (in[0]->getType())
    {
        case types::InternalType::ScilabDouble :
        {
            types::Double* pIn = in[0]->getAs<types::Double>();
            iDims = pIn->getSize();
            piDimsArray = new int[iDims];
            iProd = get_dimsarray(pIn->get(), iDims, piDimsArray);
            break;
        }
        case types::InternalType::ScilabInt8 :
        {
            types::Int8* pIn = in[0]->getAs<types::Int8>();
            iDims = pIn->getSize();
            piDimsArray = new int[iDims];
            iProd = get_dimsarray(pIn->get(), iDims, piDimsArray);
            break;
        }
        case types::InternalType::ScilabUInt8 :
        {
            types::UInt8* pIn = in[0]->getAs<types::UInt8>();
            iDims = pIn->getSize();
            piDimsArray = new int[iDims];
            iProd = get_dimsarray(pIn->get(), iDims, piDimsArray);
            break;
        }
        case types::InternalType::ScilabInt16 :
        {
            types::Int16* pIn = in[0]->getAs<types::Int16>();
            iDims = pIn->getSize();
            piDimsArray = new int[iDims];
            iProd = get_dimsarray(pIn->get(), iDims, piDimsArray);
            break;
        }
        case types::InternalType::ScilabUInt16 :
        {
            types::UInt16* pIn = in[0]->getAs<types::UInt16>();
            iDims = pIn->getSize();
            piDimsArray = new int[iDims];
            iProd = get_dimsarray(pIn->get(), iDims, piDimsArray);
            break;
        }
        case types::InternalType::ScilabInt32 :
        {
            types::Int32* pIn = in[0]->getAs<types::Int32>();
            iDims = pIn->getSize();
            piDimsArray = new int[iDims];
            iProd = get_dimsarray(pIn->get(), iDims, piDimsArray);
            break;
        }
        case types::InternalType::ScilabUInt32 :
        {
            types::UInt32* pIn = in[0]->getAs<types::UInt32>();
            iDims = pIn->getSize();
            piDimsArray = new int[iDims];
            iProd = get_dimsarray(pIn->get(), iDims, piDimsArray);
            break;
        }
        case types::InternalType::ScilabInt64 :
        {
            types::Int64* pIn = in[0]->getAs<types::Int64>();
            iDims = pIn->getSize();
            piDimsArray = new int[iDims];
            iProd = get_dimsarray(pIn->get(), iDims, piDimsArray);
            break;
        }
        case types::InternalType::ScilabUInt64 :
        {
            types::UInt64* pIn = in[0]->getAs<types::UInt64>();
            iDims = pIn->getSize();
            piDimsArray = new int[iDims];
            iProd = get_dimsarray(pIn->get(), iDims, piDimsArray);
            break;
        }
        default :
        {
            Scierror(999, _("%s: Wrong input arguments: Dimensions given as first argument do not match specified cell contents.\n"), "makecell");
            return types::Function::Error;
        }
    }

    //check vector format
    types::GenericType* pGT = in[0]->getAs<types::GenericType>();
    if (pGT->isScalar() || (pGT->getRows() != 1 && pGT->getCols() != 1))
    {
        delete[] piDimsArray;
        Scierror(999, _("%s: Wrong size for input argument #%d: A vector expected.\n"), "makecell", 1);
        return types::Function::Error;
    }

    //check input parameters count
    if (iProd != (in.size() - 1))
    {
        delete[] piDimsArray;
        Scierror(999, _("%s: Wrong input arguments: Dimensions given as first argument do not match specified cell contents.\n"), "makecell");
        return types::Function::Error;
    }

    types::Cell* pC = new types::Cell(iDims, piDimsArray);

    int i2dSize = piDimsArray[0] * piDimsArray[1];
    for (int i = 0; i < in.size() - 1; i++)
    {
        int idx2dStart = (i / i2dSize) * i2dSize;
        int idx2dRowMajor = (((i - idx2dStart) % piDimsArray[1]) * piDimsArray[0]) + ((i - idx2dStart) / piDimsArray[1]);
        int idx = idx2dRowMajor + idx2dStart;
        pC->set(idx , in[i + 1]);
    }

    delete[] piDimsArray;

    out.push_back(pC);
    return types::Function::OK;
}
