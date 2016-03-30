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
#include "double.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "elem_common.h"

    extern void C2F(dmmul)(double*, int*, double*, int*, double*, int*, int*, int*, int*);
    extern void C2F(dadd)(int*, double*, int*, double*, int*);
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_ltitr(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblX0 = NULL;

    double* pdblA   = NULL;
    double* pdblB   = NULL;
    double* pdblU   = NULL;
    double* pdblX0  = NULL;

    int iRowsA = 0;
    int iColsB = 0;
    int iColsU = 0;
    int iOne   = 1;

    if (in.size() < 3 || in.size() > 4)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "ltitr", 3, 4);
        return types::Function::Error;
    }

    if (_iRetCount < 1 || _iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "ltitr", 1, 2);
        return types::Function::Error;
    }

    /*** get inputs arguments ***/
    // get x0
    if (in.size() == 4)
    {
        if (in[3]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "ltitr", 4);
            return types::Function::Error;
        }

        pDblX0 = in[3]->getAs<types::Double>();

        if (pDblX0->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "ltitr", 4);
            return types::Function::Error;
        }

        pdblX0 = pDblX0->get();
    }

    // get U
    if (in[2]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "ltitr", 3);
        return types::Function::Error;
    }

    types::Double* pDblU = in[2]->getAs<types::Double>();

    if (pDblU->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "ltitr", 3);
        return types::Function::Error;
    }

    pdblU   = pDblU->get();
    iColsU  = pDblU->getCols();

    // get B
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "ltitr", 2);
        return types::Function::Error;
    }

    types::Double* pDblB = in[1]->getAs<types::Double>();

    if (pDblB->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "ltitr", 2);
        return types::Function::Error;
    }

    if (pDblU->getRows() != pDblB->getCols())
    {
        Scierror(999, _("%s: Wrong size for argument #%d.\n"), "ltitr", 3);
        return types::Function::Error;
    }

    pdblB   = pDblB->get();
    iColsB  = pDblB->getCols();

    // get A
    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "ltitr", 1);
        return types::Function::Error;
    }

    types::Double* pDblA = in[0]->getAs<types::Double>();

    if (pDblA->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "ltitr", 1);
        return types::Function::Error;
    }

    if (pDblA->getRows() != pDblA->getCols())
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A square matrix expected.\n"), "ltitr", 1);
        return types::Function::Error;
    }

    pdblA   = pDblA->get();
    iRowsA  = pDblA->getRows();

    if (pDblA->getRows() != pDblB->getRows())
    {
        Scierror(60, _("%s: Wrong size for argument: Incompatible dimensions.\n"), "ltitr");
        return types::Function::Error;
    }

    if (in.size() == 4)
    {
        if (pDblX0->getRows() != pDblA->getRows() || pDblX0->getCols() != 1)
        {
            Scierror(60, _("%s: Wrong size for argument: Incompatible dimensions.\n"), "ltitr");
            return types::Function::Error;
        }
    }

    /*** perform operations ***/
    double* pdblW   = new double[iRowsA];
    double* pdblWgr = new double[iRowsA * (iColsU + 1)];

    if (in.size() == 4)
    {
        C2F(dcopy)(&iRowsA, pdblX0, &iOne, pdblWgr, &iOne);
    }
    else
    {
        memset(pdblWgr, 0x00, iRowsA * sizeof(double));
    }

    int ig = 0;
    for (int i = 0; i < iColsU; i++)
    {
        ig = (i + 1) * iRowsA;
        C2F(dmmul)(pdblA, &iRowsA, pdblWgr + ig - iRowsA, &iRowsA, pdblW, &iRowsA, &iRowsA, &iRowsA, &iOne);
        C2F(dmmul)(pdblB, &iRowsA, pdblU + (i * iColsB), &iColsB, pdblWgr + ig, &iRowsA, &iRowsA, &iColsB, &iOne);
        C2F(dadd)(&iRowsA, pdblW, &iOne, pdblWgr + ig, &iOne);
    }

    /*** return output arguments ***/
    if (_iRetCount == 2)
    {
        types::Double* pDblOutXf = new types::Double(iRowsA, 1);
        double* pdblOutXf = pDblOutXf->get();
        C2F(dcopy)(&iRowsA, pdblWgr + iColsU, &iOne, pdblOutXf, &iOne);
        out.push_back(pDblOutXf);
    }

    types::Double* pDblOutX = new types::Double(iRowsA, iColsU);
    double* pdblOutX = pDblOutX->get();
    int iSize = pDblOutX->getSize();
    C2F(dcopy)(&iSize, pdblWgr, &iOne, pdblOutX, &iOne);
    out.push_back(pDblOutX);


    delete[] pdblW;
    delete[] pdblWgr;

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
