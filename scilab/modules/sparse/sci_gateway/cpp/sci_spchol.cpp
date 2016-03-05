/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
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
#include <Eigen/Sparse>

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
}

types::Function::ReturnValue sci_spchol(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Sparse* pSpIn       = NULL;
    types::Sparse* pSpPermut   = NULL;
    types::Sparse* pSpFact     = NULL;

    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "spchol", 1);
        return types::Function::Error;
    }

    if (_iRetCount != 2)
    {
        Scierror(999, _("%s: Wrong number of output argument(s): %d expected.\n"), "spchol", 2);
        return types::Function::Error;
    }

    if (in[0]->isSparse() == false && in[0]->isSparseBool() == false)
    {
        Scierror(999, _("%s: Wrong type for argument #%d: Sparse matrix expected.\n"), "spchol", 1);
        return types::Function::Error;
    }

    pSpIn = in[0]->getAs<types::Sparse>();

    if (pSpIn->isComplex())
    {
        Scierror(999, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), "spchol", 1);
        return types::Function::Error;
    }

    if (pSpIn->getRows() != pSpIn->getCols())
    {
        Scierror(999, _("%s: Wrong size for argument #%d: Square sparse matrix expected.\n"), "spchol", 1);
        return types::Function::Error;
    }

    int ierr = pSpIn->newCholLLT(&pSpPermut, &pSpFact);

    switch (ierr)
    {
        case Eigen::Success :
            break;
        case Eigen::NumericalIssue :
            Scierror(999, _("%s: The provided data did not satisfy the prerequisites.\n"), "spchol");
            return types::Function::Error;
        case Eigen::NoConvergence :
            Scierror(999, _("%s: Iterative procedure did not converge.\n"), "spchol");
            return types::Function::Error;
        case Eigen::InvalidInput :
            Scierror(999, _("%s: The inputs are invalid, or the algorithm has been improperly called.\nWhen assertions are enabled, such errors trigger an assert.\n"), "spchol");
            return types::Function::Error;
        default :
            break;
    }

    out.push_back(pSpFact);
    out.push_back(pSpPermut);

    return types::Function::OK;
}
