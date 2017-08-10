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
#include "polynom.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "elem_common.h"

    extern void C2F(dtild)(int*, double*, int*);
    extern void C2F(expan)(double*, int*, double*, int*, double*, int*);
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_ldiv(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    double** pdblCoef1  = NULL;
    double** pdblCoef2  = NULL;
    int* piRank1        = NULL;
    int* piRank2        = NULL;
    int iSize           = 0;
    int iK              = 0;
    int iRows           = 0;
    int iCols           = 0;
    int iOne            = 1;

    if (in.size() != 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "ldiv", 3);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "ldiv", 1);
        return types::Function::Error;
    }

    /*** get inputs arguments ***/
    // get first polynom
    if (in[0]->isPoly())
    {
        types::Polynom* pPoly1 = in[0]->getAs<types::Polynom>();
        if (pPoly1->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real polynom expected.\n"), "ldiv", 1);
            return types::Function::Error;
        }

        iSize = pPoly1->getSize();
        iRows = pPoly1->getRows();
        iCols = pPoly1->getCols();

        piRank1 = new int[iSize];
        pPoly1->getRank(piRank1);

        pdblCoef1 = new double*[iSize];
        for (int i = 0; i < iSize; i++)
        {
            pdblCoef1[i] = pPoly1->get(i)->get();
        }
    }
    else if (in[0]->isDouble())
    {
        types::Double* pDbl1 = in[0]->getAs<types::Double>();
        if (pDbl1->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "ldiv", 1);
            return types::Function::Error;
        }

        iSize = pDbl1->getSize();
        iRows = pDbl1->getRows();
        iCols = pDbl1->getCols();

        piRank1 = new int[iSize];
        memset(piRank1, 0x00, iSize * sizeof(int));

        pdblCoef1 = new double*[iSize];
        double* pdbl = pDbl1->get();
        for (int i = 0; i < iSize; i++)
        {
            pdblCoef1[i] = pdbl + i;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix or polynom expected.\n"), "ldiv", 1);
        return types::Function::Error;
    }

    // get second polynom
    if (in[1]->isPoly())
    {
        types::Polynom* pPoly2 = in[1]->getAs<types::Polynom>();
        if (pPoly2->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real polynom expected.\n"), "ldiv", 2);
            delete[] pdblCoef1;
            delete[] piRank1;
            return types::Function::Error;
        }

        if (pPoly2->getRows() != iRows || pPoly2->getCols() != iCols)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A same size as input argument %d expected.\n"), "ldiv", 2, 1);
            delete[] pdblCoef1;
            delete[] piRank1;
            return types::Function::Error;
        }

        piRank2 = new int[iSize];
        pPoly2->getRank(piRank2);

        pdblCoef2 = new double*[iSize];
        for (int i = 0; i < iSize; i++)
        {
            pdblCoef2[i] = pPoly2->get(i)->get();
        }
    }
    else if (in[1]->isDouble())
    {
        types::Double* pDbl2 = in[1]->getAs<types::Double>();
        if (pDbl2->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "ldiv", 2);
            delete[] pdblCoef1;
            delete[] piRank1;
            return types::Function::Error;
        }

        if (pDbl2->getRows() != iRows || pDbl2->getCols() != iCols)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A same size as input argument %d expected.\n"), "ldiv", 2, 1);
            delete[] pdblCoef1;
            delete[] piRank1;
            return types::Function::Error;
        }

        piRank2 = new int[iSize];
        memset(piRank2, 0x00, iSize * sizeof(int));

        pdblCoef2 = new double*[iSize];
        double* pdbl = pDbl2->get();
        for (int i = 0; i < iSize; i++)
        {
            pdblCoef2[i] = pdbl + i;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix or polynom expected.\n"), "ldiv", 2);
        delete[] pdblCoef1;
        delete[] piRank1;
        return types::Function::Error;
    }

    // get k
    if (in[2]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "ldiv", 3);
        delete[] pdblCoef1;
        delete[] pdblCoef2;
        delete[] piRank1;
        delete[] piRank2;
        return types::Function::Error;
    }

    types::Double* pDbl = in[2]->getAs<types::Double>();

    if (pDbl->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real scalar expected.\n"), "ldiv", 3);
        delete[] pdblCoef1;
        delete[] pdblCoef2;
        delete[] piRank1;
        delete[] piRank2;
        return types::Function::Error;
    }

    if (pDbl->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), "ldiv", 3);
        delete[] pdblCoef1;
        delete[] pdblCoef2;
        delete[] piRank1;
        delete[] piRank2;
        return types::Function::Error;
    }

    iK = (int)pDbl->get(0);

    /*** perform operations ***/
    types::Double* pDblOut = new types::Double(iRows * iK, iCols);
    double* pdblout = pDblOut->get();

    for (int i = 0; i < iSize; i++)
    {
        int iSize1 = piRank1[i] + 1;
        int iSize2 = piRank2[i] + 1;
        double* temp1 = new double[iSize1];
        double* temp2 = new double[iSize2];
        C2F(dcopy)(&iSize1, pdblCoef1[i], &iOne, temp1, &iOne);
        C2F(dcopy)(&iSize2, pdblCoef2[i], &iOne, temp2, &iOne);
        C2F(dtild)(&iSize1, temp1, &iOne);
        C2F(dtild)(&iSize2, temp2, &iOne);
        C2F(expan)(temp2, &iSize2, temp1, &iSize1, pdblout, &iK);
        delete[] temp1;
        delete[] temp2;
    }

    delete[] pdblCoef1;
    delete[] pdblCoef2;
    delete[] piRank1;
    delete[] piRank2;

    /*** retrun output arguments ***/
    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
