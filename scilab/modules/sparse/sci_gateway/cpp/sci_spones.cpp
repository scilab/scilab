/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bernard HUGUENEY
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
