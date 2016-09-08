/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) Scilab Enterprises - 2016 - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/* ========================================================================== */
#include "mpi_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "sciMpiComm.hxx"
#include "mpi.h"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

static const char fname[] = "MPI_Create_comm";
/* ========================================================================== */
types::Function::ReturnValue sci_mpi_createComm(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), fname, 1);
        return types::Function::Error;
    }

    if (out.size() > 1)
    {
        Scierror(999, _("%s: Wrong number of output argument(s): %d expected.\n"), fname, 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false )
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A Matrix expected.\n"), fname, 1);
        return types::Function::Error;
    }

    types::Double* pDbl = in[0]->getAs<types::Double>();
    int iSize = pDbl->getSize();
    int* piRanks = new int[iSize];
    double* pdblRanks = pDbl->get();
    for (int i = 0; i < iSize; i++)
    {
        piRanks[i] = (int)pdblRanks[i];
    }

    out.push_back(new SciMpiComm(piRanks, iSize));

    delete[] piRanks;
    return types::Function::OK;
}
/* ========================================================================== */
