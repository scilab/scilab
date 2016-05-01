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

    extern void C2F(ereduc)(double*, int*, int*, double*, double*, int*, int*, double*);
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_ereduc(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    double* pdblX   = NULL;
    int iRowsX      = 0;
    int iColsX      = 0;
    double dTol     = 0;

    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "ereduc", 2);
        return types::Function::Error;
    }

    if (_iRetCount < 1 || _iRetCount > 5)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "ereduc", 1, 5);
        return types::Function::Error;
    }

    /*** get inputs arguments ***/
    // get X
    if (in[0]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_ereduc";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    types::Double* pDblX = in[0]->clone()->getAs<types::Double>();
    pdblX   = pDblX->get();
    iColsX  = pDblX->getCols();
    iRowsX  = pDblX->getRows();

    // get Tol
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "ereduc", 2);
        return types::Function::Error;
    }

    types::Double* pDblTol = in[1]->getAs<types::Double>();

    if (pDblTol->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "ereduc", 2);
        return types::Function::Error;
    }

    dTol = pDblTol->get(0);

    /*** perform operations ***/
    types::Double* pDblQ = new types::Double(iRowsX, iRowsX);
    double* pdblQ = pDblQ->get();
    types::Double* pDblZ = new types::Double(iColsX, iColsX);
    double* pdblZ = pDblZ->get();

    int* piStair = new int[iRowsX];
    int iRk = 0;

    C2F(ereduc)(pdblX, &iRowsX, &iColsX, pdblQ, pdblZ, piStair, &iRk, &dTol);

    /*** retrun output arguments ***/
    // return E
    out.push_back(pDblX);

    // return Q
    if (_iRetCount > 1)
    {
        out.push_back(pDblQ);
    }
    else
    {
        delete pDblQ;
    }

    // return Z
    if (_iRetCount > 2)
    {
        out.push_back(pDblZ);
    }
    else
    {
        delete pDblZ;
    }

    // return Stair
    if (_iRetCount > 3)
    {
        types::Double* pDblStair = new types::Double(1, iRowsX);
        double* pdblStair = pDblStair->get();
        for (int i = 0; i < iRowsX; i++)
        {
            pdblStair[i] = (double)(piStair[i]);
        }

        out.push_back(pDblStair);
    }

    delete[] piStair;

    // return rk
    if (_iRetCount > 4)
    {
        out.push_back(new types::Double((double)iRk));
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
