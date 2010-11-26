/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bernard HUGUENEY
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "alltypes.hxx"
#include "sparse.hxx"
#include "funcmanager.hxx"
extern "C" {
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
}

using namespace types;

Function::ReturnValue sci_spones(typed_list &in, int _piRetCount, typed_list &out)
{
    types::GenericType *pRetVal = NULL;

    if ( in.size() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"spones", 1);
        return Function::Error;
    }
    if ( (in[0]->getType() != InternalType::RealSparse) && (in[0]->getType() != InternalType::RealSparseBool))
    {
        ScierrorW(999, _W("%ls: Wrong type for argument %d: Sparse matrix expected.\n"), L"spones", 1);
        return Function::Error;
    }
    pRetVal = in[0]->isSparse() ? in[0]->getAsSparse()->newOnes() : in[0]->getAsSparseBool()->newOnes();
    if(pRetVal == 0)
    {
        return Function::Error;
    }

    out.push_back(pRetVal);

    return Function::OK;
}
