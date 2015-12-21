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

#include "sparse_gw.hxx"
#include "function.hxx"
#include "sparse.hxx"

extern "C"
{
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
}

types::Function::ReturnValue sci_spones(types::typed_list &in, int _piRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "spones", 1);
        return types::Function::Error;
    }

    if (in[0]->isSparse() == false && in[0]->isSparseBool() == false)
    {
        Scierror(999, _("%s: Wrong type for argument %d: Sparse matrix expected.\n"), "spones", 1);
        return types::Function::Error;
    }

    if (in[0]->isSparse())
    {
        out.push_back(in[0]->getAs<types::Sparse>()->newOnes());
    }
    else
    {
        out.push_back(in[0]->getAs<types::SparseBool>()->newOnes());
    }

    return types::Function::OK;
}
