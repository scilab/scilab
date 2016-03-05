/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bernard HUGUENEY
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
#include "double.hxx"
#include "bool.hxx"

extern "C"
{
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
}

types::Function::ReturnValue sci_full(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "full", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "full", 1);
    }

    if (in[0]->isSparse())
    {
        types::Sparse* pSp = in[0]->getAs<types::Sparse>();
        types::Double* pOut = NULL;

        if (pSp->getRows() == 0 && pSp->getCols() == 0)
        {
            pOut = types::Double::Empty();
        }
        else
        {
            pOut = new types::Double(pSp->getRows(), pSp->getCols(), pSp->isComplex());
            pSp->fill(*pOut);
        }

        out.push_back(pOut);
    }
    else if (in[0]->isSparseBool())
    {
        types::SparseBool* pSb = in[0]->getAs<types::SparseBool>();
        if (pSb->getRows() == 0 && pSb->getCols() == 0)
        {
            out.push_back(types::Double::Empty());
        }
        else
        {
            types::Bool* pOut = new types::Bool(pSb->getRows(), pSb->getCols());
            pSb->fill(*pOut);
            out.push_back(pOut);
        }
    }
    else
    {
        out.push_back(in[0]->clone());
    }

    return types::Function::OK;
}
