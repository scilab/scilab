/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Cedric DELAMARRE
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
/*--------------------------------------------------------------------------*/
#include "cacsd_gw.hxx"
#include "function.hxx"
#include "overload.hxx"
#include "double.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"

    extern void C2F(fstair)(double*, double*, double*, double*, int*, int*, int*, int*, double*,
                            int*, int*, int*, int*, int*, int*, double*, int*, int*);
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_fstair(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    //     Inputs           A,    E,    Q,    Z,   stair, rk, tol
    double* pdblIn[7]   = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    int iRows[7]        = {0, 0, 0, 0, 0, 0, 0};
    int iCols[7]        = {0, 0, 0, 0, 0, 0, 0};

    //     Output                   AE,   EE,   QE,   ZE,  blcks, muk, nuk,  muk0, nuk0, mnei
    types::Double* pDblOut[10]  = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    double* pdblOut[10]         = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    int piSizeOut[10]           = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    if (in.size() != 7)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "fstair", 7);
        return types::Function::Error;
    }

    if (_iRetCount < 1 || _iRetCount > 10)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "fstair", 1, 10);
        return types::Function::Error;
    }

    /*** get inputs arguments ***/
    // get the 4 input/output arguments
    for (int i = 0; i < 4; i++)
    {
        if (in[i]->isDouble() == false)
        {
            std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_fstair";
            return Overload::call(wstFuncName, in, _iRetCount, out);
        }

        pDblOut[i] = in[i]->clone()->getAs<types::Double>();
        pdblIn[i] = pDblOut[i]->get();
        iRows[i]  = pDblOut[i]->getRows();
        iCols[i]  = pDblOut[i]->getCols();

        pdblOut[i] = pdblIn[i];
        piSizeOut[i] = pDblOut[i]->getSize();
    }

    // get next input arguemnts
    for (int i = 4; i < 7; i++)
    {
        if (in[i]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "fstair", i + 1);
            return types::Function::Error;
        }

        types::Double* pDblIn = in[i]->getAs<types::Double>();
        pdblIn[i] = pDblIn->get();
        iRows[i]  = pDblIn->getRows();
        iCols[i]  = pDblIn->getCols();
    }

    // check size of input arguments
    if (iRows[2] != iCols[2])
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A square matrix expected.\n"), "fstair", 3);
        return types::Function::Error;
    }

    if (iRows[3] != iCols[3])
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A square matrix expected.\n"), "fstair", 4);
        return types::Function::Error;
    }

    if (iRows[4] != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A row vector expected.\n"), "fstair", 5);
        return types::Function::Error;
    }

    if (iRows[5] * iCols[5] != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "fstair", 6);
        return types::Function::Error;
    }

    if (iRows[6] * iCols[6] != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "fstair", 7);
        return types::Function::Error;
    }

    if (iRows[0] != iRows[1] || iRows[0] != iRows[2] || iRows[0] != iCols[2] || iRows[0] != iCols[4] ||
            iCols[0] != iCols[1] || iCols[0] != iCols[3] || iCols[0] != iRows[3])
    {
        Scierror(999, _("%s: Incompatible input argument.\n"), "fstair");
        return types::Function::Error;
    }

    /*** perform operations ***/
    int iErr = 0;
    int* piOut[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

    // convert input argument 5 from double to int
    int* piIn5 = new int[iCols[4]];
    for (int i = 0; i < iCols[4]; i++)
    {
        piIn5[i] = (int)pdblIn[4][i];
    }

    // convert input argument 6 from double to int
    int iIn6 = (int)pdblIn[5][0];

    // create output blcks, muk, nuk, muk0, nuk0, mnei
    pDblOut[4] = new types::Double(1, 1);
    pDblOut[5] = new types::Double(1, iCols[0]);
    pDblOut[6] = new types::Double(1, iRows[0] + 1);
    pDblOut[7] = new types::Double(1, iCols[0]);
    pDblOut[8] = new types::Double(1, iRows[0] + 1);
    pDblOut[9] = new types::Double(4, 1);

    for (int i = 4; i < 10; i++)
    {
        pdblOut[i]      = pDblOut[i]->get();
        piSizeOut[i]    = pDblOut[i]->getSize();
        piOut[i - 4]      = new int[piSizeOut[i]];
    }

    // create working arrays
    double* pdblWork = new double[iCols[0]];
    int* piWork      = new int[iCols[0]];

    // call function fstair
    C2F(fstair)(pdblIn[0], pdblIn[1], pdblIn[2], pdblIn[3], iRows, iCols, piIn5, &iIn6, pdblIn[6],
                piOut[0], piOut[1], piOut[2], piOut[3], piOut[4], piOut[5], pdblWork, piWork, &iErr);

    delete[] pdblWork;
    delete[] piWork;
    delete[] piIn5;

    if (iErr)
    {
        Scierror(999, _("%s: The algorithm has failed.\n"), "fstair");
        for (int i = 0; i < 10; i++)
        {
            delete[] pDblOut[i];
        }

        for (int i = 0; i < 6; i++)
        {
            delete[] piOut[i];
        }

        return types::Function::Error;
    }

    /*** retrun output arguments ***/
    // convert blcks, muk, nuk, muk0, nuk0, mnei from int to double
    for (int i = 4; i < 10; i++)
    {
        for (int j = 0; j < piSizeOut[i]; j++)
        {
            pdblOut[i][j] = (double)(piOut[i - 4][j]);
        }

        delete[] piOut[i - 4];
    }

    // retrun output arguments
    for (int i = 0; i < 10; i++)
    {
        out.push_back(pDblOut[i]);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
