/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Cedric DELAMARRE
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
#include "optimization_gw.hxx"
#include "function.hxx"
#include "double.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
    extern void C2F(spf)(int*, int*, double*, int*, double*, double*, double*,
                         double*, double*, double*, double*, double*, int*,
                         double*, int*, int*, int*);
}


/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_semidef(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblX    = NULL;
    types::Double* pDblZ    = NULL;
    types::Double* pDblF    = NULL;
    types::Double* pDblB    = NULL;
    types::Double* pDblC    = NULL;
    types::Double* pDblOpt  = NULL;

    int* piB = NULL;

    double dNu      = 0;
    double dAbstol  = 0;
    double dReltol  = 0;
    double dTv      = 0;
    double dIter    = 0;

    int iSizeX = 0;
    int iSizeB = 0;
    int iIter  = 0;
    int iInfo  = 0;

    if (in.size() < 1 || in.size() > 6)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "semidef", 1, 6);
        return types::Function::Error;
    }

    if (_iRetCount > 4)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "semidef", 1, 4);
        return types::Function::Error;
    }

    /*** get inputs arguments ***/
    // get X
    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "semidef", 1);
        return types::Function::Error;
    }

    pDblX = in[0]->clone()->getAs<types::Double>();

    if (pDblX->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "semidef", 1);
        return types::Function::Error;
    }

    iSizeX = pDblX->getSize();

    // get Z
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "semidef", 2);
        return types::Function::Error;
    }

    pDblZ = in[1]->clone()->getAs<types::Double>();

    if (pDblZ->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "semidef", 2);
        return types::Function::Error;
    }

    // get F
    if (in[2]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "semidef", 3);
        return types::Function::Error;
    }

    pDblF = in[2]->getAs<types::Double>();

    if (pDblF->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "semidef", 3);
        return types::Function::Error;
    }

    // get blocksize
    if (in[3]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "semidef", 4);
        return types::Function::Error;
    }

    pDblB = in[3]->getAs<types::Double>();

    if (pDblB->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "semidef", 4);
        return types::Function::Error;
    }

    double* pdblB = pDblB->get();
    iSizeB = pDblB->getSize();

    // get C
    if (in[4]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "semidef", 5);
        return types::Function::Error;
    }

    pDblC = in[4]->getAs<types::Double>();

    if (pDblC->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "semidef", 5);
        return types::Function::Error;
    }

    if (pDblC->getRows()* pDblC->getCols() != pDblX->getRows()*pDblX->getCols())
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Input argument %d size expected.\n"), "semidef", 5, 1);
        return types::Function::Error;
    }

    // get Option
    if (in[5]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "semidef", 6);
        return types::Function::Error;
    }

    pDblOpt = in[5]->getAs<types::Double>();

    if (pDblOpt->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real vector expected.\n"), "semidef", 6);
        return types::Function::Error;
    }

    if (pDblOpt->getRows() != 1 || pDblOpt->getCols() != 5)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A vector of size %d x %d expected.\n"), "semidef", 6, 1, 5);
        return types::Function::Error;
    }

    dNu      = pDblOpt->get(0);
    dAbstol  = pDblOpt->get(1);
    dReltol  = pDblOpt->get(2);
    dTv      = pDblOpt->get(3);
    iIter    = (int)pDblOpt->get(4);

    /*** perform operations ***/
    types::Double* pDblUl = new types::Double(1, 2);
    int* piWork = new int[iSizeX];

    // compute size of working table
    int n       = 0;
    int sz      = 0;
    int upsz    = 0;
    int maxn    = 0;
    piB = new int[iSizeB];
    for (int i = 0; i < iSizeB; i++)
    {
        piB[i] = (int)pdblB[i];
    }

    for (int i = 0; i < iSizeB; i++)
    {
        n += piB[i];
        sz += piB[i] * (piB[i] + 1) / 2;
        upsz += piB[i] * piB[i];
        maxn = std::max(maxn, piB[i]);
    }

    // optimal block size for dgels ????
    int nb = 32;
    int iSizeWork = (iSizeX + 2) * sz +
                    upsz + 2 * n +
                    std::max(std::max(iSizeX + sz * nb, 3 * maxn + maxn * (maxn + 1)), 3 * iSizeX);

    double* pdblWork = new double[iSizeWork];
    C2F(spf)(&iSizeX, &iSizeB, pDblF->get(), piB, pDblC->get(), pDblX->get(), pDblZ->get(),
             pDblUl->get(), &dNu, &dAbstol, &dReltol, &dTv, &iIter, pdblWork, &iSizeWork, piWork, &iInfo);

    delete[] pdblWork;
    delete[] piWork;
    delete[] piB;

    if (iInfo < 0)
    {
        if (iInfo == -18)
        {
            Scierror(999, _("semi def fails.\n"));
            return types::Function::Error;
        }
        else
        {
            Scierror(999, _("%s: Input argument %d of %s function has an illegal value.\n"), "semidef", -iInfo, "spf");
            return types::Function::Error;
        }
    }

    /*** return output arguments ***/
    out.push_back(pDblX);

    if (_iRetCount >= 2)
    {
        out.push_back(pDblZ);
    }
    else
    {
        delete pDblZ;
    }

    if (_iRetCount >= 3)
    {
        out.push_back(pDblUl);
    }
    else
    {
        delete pDblUl;
    }

    if (_iRetCount == 4)
    {
        types::Double* pDblOut = new types::Double(1, 2);
        pDblOut->set(0, (double)iInfo);
        pDblOut->set(1, (double)iIter);

        out.push_back(pDblOut);
    }

    return types::Function::OK;
}

