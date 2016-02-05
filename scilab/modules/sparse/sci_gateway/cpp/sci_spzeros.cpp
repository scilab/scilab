/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
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

#include "sparse_gw.hxx"
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
    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "spzeros", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "spzeros", 1);
        return types::Function::Error;
    }

    if (in.size() == 2)
    {
        types::Double* pDblRows = NULL;
        types::Double* pDblCols = NULL;

        if (in[0]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "spzeros", 1);
            return types::Function::Error;
        }

        if (in[1]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "spzeros", 2);
            return types::Function::Error;
        }

        pDblRows = in[0]->getAs<types::Double>();
        pDblCols = in[1]->getAs<types::Double>();

        if (pDblRows->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "spzeros", 1);
            return types::Function::Error;
        }

        if (pDblCols->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "spzeros", 2);
            return types::Function::Error;
        }

        pSpOut = new types::Sparse((int)pDblRows->get(0), (int)pDblCols->get(0), false);

    }
    else // in.size() == 1
    {
        switch (in[0]->getType())
        {
            case types::InternalType::ScilabInt8 :
            case types::InternalType::ScilabUInt8 :
            case types::InternalType::ScilabInt16 :
            case types::InternalType::ScilabUInt16 :
            case types::InternalType::ScilabInt32 :
            case types::InternalType::ScilabUInt32 :
            case types::InternalType::ScilabInt64 :
            case types::InternalType::ScilabUInt64 :
            case types::InternalType::ScilabString :
            case types::InternalType::ScilabDouble :
            case types::InternalType::ScilabBool :
            case types::InternalType::ScilabFloat :
            case types::InternalType::ScilabPolynom :
            case types::InternalType::ScilabSinglePolynom :
            case types::InternalType::ScilabSparse :
            case types::InternalType::ScilabSparseBool :
                break;
            default :
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "spzeros", 1);
                return types::Function::Error;
            }
        }

        types::GenericType* pGT = in[0]->getAs<types::GenericType>();
        pSpOut = new types::Sparse(pGT->getRows(), pGT->getCols(), false);

    }

    out.push_back(pSpOut);
    return types::Function::OK;
}
