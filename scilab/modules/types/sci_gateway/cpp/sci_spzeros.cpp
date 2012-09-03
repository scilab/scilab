/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_gw.hxx"
#include "function.hxx"
#include "sparse.hxx"

extern "C"
{
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
}

types::Function::ReturnValue sci_spzeros(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Sparse *pSpOut = NULL;
    if (in.size() > 2)
    {
        ScierrorW(999, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"spzeros", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of output arguments: %d expected.\n"), L"spzeros", 1);
        return types::Function::Error;
    }

    if (in.size() == 2)
    {
        types::Double* pDblRows = NULL;
        types::Double* pDblCols = NULL;

        if (in[0]->isDouble() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A scalar expected.\n"), L"spzeros", 1);
            return types::Function::Error;
        }

        if (in[1]->isDouble() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A scalar expected.\n"), L"spzeros", 2);
            return types::Function::Error;
        }

        pDblRows = in[0]->getAs<types::Double>();
        pDblCols = in[1]->getAs<types::Double>();

        if (pDblRows->isScalar() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A scalar expected.\n"), L"spzeros", 1);
            return types::Function::Error;
        }

        if (pDblCols->isScalar() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A scalar expected.\n"), L"spzeros", 2);
            return types::Function::Error;
        }

        pSpOut = new types::Sparse(pDblRows->get(0), pDblCols->get(0), false);

    }
    else // in.size() == 1
    {
        switch (in[0]->getType())
        {
            case types::InternalType::RealInt8 :
            case types::InternalType::RealUInt8 :
            case types::InternalType::RealInt16 :
            case types::InternalType::RealUInt16 :
            case types::InternalType::RealInt32 :
            case types::InternalType::RealUInt32 :
            case types::InternalType::RealInt64 :
            case types::InternalType::RealUInt64 :
            case types::InternalType::RealString :
            case types::InternalType::RealDouble :
            case types::InternalType::RealBool :
            case types::InternalType::RealFloat :
            case types::InternalType::RealPoly :
            case types::InternalType::RealSinglePoly :
            case types::InternalType::RealSparse :
            case types::InternalType::RealSparseBool :
                break;
            default :
            {
                ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A matrix expected.\n"), L"spzeros", 1);
                return types::Function::Error;
            }
        }

        types::GenericType* pGT = in[0]->getAs<types::GenericType>();
        pSpOut = new types::Sparse(pGT->getRows(), pGT->getCols(), false);

    }

    out.push_back(pSpOut);
    return types::Function::OK;
}
