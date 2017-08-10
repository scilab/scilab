/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
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
#include "types.hxx"
#include "double.hxx"
#include "sparse.hxx"
#include "pointer.hxx"
#include "overload.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "elem_common.h"
#include "lu.h"
}

types::Function::ReturnValue sci_lusolve(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    double abstol   = 0;
    double reltol   = 0.001;
    int nrank       = 0;
    int ierr        = 0;
    int m1          = 0;
    int n1          = 0;
    int m2          = 0;
    int n2          = 0;
    int nonZeros    = 0;
    int fmatindex   = 0;
    bool fact       = false;

    const void *pData   = NULL;
    double* dbl         = NULL;
    int* colPos         = NULL;
    int* itemsRow       = NULL;

    //check input parameters
    if (in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "lusolve", 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "lusolve", 1);
        return types::Function::Error;
    }

    if (in[0]->isPointer())
    {
        types::Pointer *pPointerIn = in[0]->getAs<types::Pointer>();
        m1 = pPointerIn->getRows();
        n1 = pPointerIn->getCols();
        pData = pPointerIn->get();
        fmatindex = *(int*)pData;
        fact = false;
    }
    else if (in[0]->isSparse())
    {
        types::Sparse *pSpIn = in[0]->getAs<types::Sparse>();
        m1 = pSpIn->getRows();
        n1 = pSpIn->getCols();
        if (m1 != n1)
        {
            Scierror(77, _("%s: Wrong size for input argument #%d: Square matrix expected.\n"), "lusolve", 1);
            return types::Function::Error;
        }

        if (pSpIn->isComplex())
        {
            Scierror(77, _("%s: Wrong type for argument %d: Real matrix expected.\n"), "lusolve", 1);
            return types::Function::Error;
        }

        nonZeros = (int)pSpIn->nonZeros();
        dbl = new double[nonZeros];
        pSpIn->outputValues(dbl, NULL);
        colPos = new int[nonZeros];
        itemsRow = new int[m1];
        pSpIn->getColPos(colPos);
        pSpIn->getNbItemByRow(itemsRow);

        abstol = nc_eps_machine();
        C2F(lufact1)(dbl, itemsRow, colPos, &m1, &nonZeros, &fmatindex, &abstol, &reltol, &nrank, &ierr);
        fact = true;

        delete[] dbl;
        delete[] colPos;
        delete[] itemsRow;
    }
    else
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_lusolve";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    if ((in[1]->isSparse() == false) && (in[1]->isDouble() == false))
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_lusolve";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    if (in[1]->isSparse() )
    {
        Scierror(999, _("%s not yet implemented for full input parameter.\n"), "lusolve");
        return types::Function::Error;
    }

    if (in[1]->isDouble() )
    {
        types::Double *pDblIn = in[1]->getAs<types::Double>();

        m2 = pDblIn->getRows();
        n2 = pDblIn->getCols();

        if (m2 != m1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Incompatible dimensions.\n"), "lusolve", 2);
            return types::Function::Error;
        }

        double *dbl  = pDblIn->getReal();
        types::Double *pDblOut = new types::Double(m2, n2, pDblIn->isComplex());
        double *oReal = pDblOut->get();

        if (pDblIn->isComplex())
        {
            double *imag = pDblIn->getImg();
            double *oImg = pDblOut->getImg();
            for (int i = 0; i < n2; i++)
            {
                int iPos = i * m2;
                C2F(lusolve1)(&fmatindex, dbl + iPos, oReal + iPos, &ierr);
                if (ierr > 0)
                {
                    Scierror(999, _("Wrong value for argument #%d: the lu handle is no more valid.\n"), 1);
                    return types::Function::Error;
                }
                C2F(lusolve1)(&fmatindex, imag + iPos, oImg + iPos, &ierr);
                if (ierr > 0)
                {
                    Scierror(999, _("Wrong value for argument #%d: the lu handle is no more valid.\n"), 1);
                    return types::Function::Error;
                }
            }
        }
        else
        {
            for (int i = 0; i < n2; i++)
            {
                C2F(lusolve1)(&fmatindex, &dbl[i * m2], &oReal[i * m2], &ierr);
                if (ierr > 0)
                {
                    Scierror(999, _("Wrong value for argument #%d: the lu handle is no more valid.\n"), 1);
                    return types::Function::Error;
                }
            }
        }

        if (fact)
        {
            C2F(ludel1)(&fmatindex, &ierr);
        }

        out.push_back(pDblOut);
    }

    return types::Function::OK;
}
