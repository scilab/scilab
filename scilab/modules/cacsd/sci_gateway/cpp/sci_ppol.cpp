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

    extern void C2F(ssxmc)( int*, int*, double*, int*, double*, int*, int*, int*, double*,
                            double*, double*, double*, double*, double*, int*);

    extern void C2F(polmc)( int*, int*, int*, int*, double*, double*, double*, double*, double*, double*,
                            int*, int*, int*, double*, double*, double*, double*, double*, double*, double*);

}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_ppol(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblA = NULL;
    types::Double* pDblB = NULL;
    types::Double* pDblP = NULL;

    double dblEps = 0.1 * sqrt(nc_eps_machine());

    int iOne   = 1;
    int iSizeP = 0;
    int iColB  = 0;

    bool isDeletable = false;

    if (in.size() != 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "ppol", 3);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "ppol", 1);
        return types::Function::Error;
    }

    /*** get inputs arguments ***/
    // get poles
    if (in[2]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "ppol", 3);
        return types::Function::Error;
    }

    pDblP = in[2]->getAs<types::Double>();
    iSizeP = pDblP->getSize();

    // get matrix B
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "ppol", 2);
        return types::Function::Error;
    }

    pDblB = in[1]->clone()->getAs<types::Double>();

    if (pDblB->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "ppol", 2);
        return types::Function::Error;
    }

    iColB = pDblB->getCols();

    // get matrix A
    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "ppol", 1);
        return types::Function::Error;
    }

    pDblA = in[0]->clone()->getAs<types::Double>();

    if (pDblA->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "ppol", 1);
        return types::Function::Error;
    }

    if (pDblA->getRows() != pDblA->getCols())
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A square matrix expected.\n"), "ppol", 1);
        return types::Function::Error;
    }

    if (pDblA->getRows() != iSizeP || pDblB->getRows() != iSizeP)
    {
        Scierror(999, _("%s: Wrong size for argument: Incompatible dimensions.\n"), "ppol");
        return types::Function::Error;
    }

    if (iSizeP == 0)
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    /*** perform operations ***/
    types::Double* pDblOut = new types::Double(iColB, iSizeP);

    double* pdblG   = pDblOut->get();
    double* pdblZ   = new double[iSizeP * iSizeP];
    int iSizeWork   = std::max(iSizeP * iColB + 3 * iColB,
                               iColB * iColB + iColB * std::max(2, iColB) + 3 * iColB + 2 * iSizeP);
    double* pdblW   = new double[iSizeWork];
    int* piW        = new int[iSizeP];

    double* pdblPReal = pDblP->get();
    double* pdblPImg  = NULL;
    if (pDblP->isComplex())
    {
        pdblPImg = pDblP->getImg();
    }
    else
    {
        pdblPImg = new double[iSizeP];
        memset(pdblPImg, 0x00, iSizeP * sizeof(double));
        isDeletable = true;
    }

    int idc = 0;
    int inc = 0;
    // calcul de la forme canonique orthogonale
    C2F(ssxmc)(&iSizeP, &iColB, pDblA->get(), &iSizeP, pDblB->get(), &idc, &inc, piW, pdblZ,
               pdblW + iColB, pdblW + iColB + iSizeP * iColB, pdblW + 2 * iColB + iSizeP * iColB,
               pdblW, &dblEps, &iOne);

    if (idc < iSizeP)
    {
        Scierror(999, _("%s: Uncontrollable system.\n"), "ppol");
        delete[] pdblZ;
        delete[] pdblW;
        delete[] piW;
        if (isDeletable)
        {
            delete[] pdblPImg;
        }
        delete pDblA;
        delete pDblB;
        return types::Function::Error;
    }

    double* pdblW1 = pdblW  + iColB + iColB * iColB;
    double* pdblW2 = pdblW1 + iColB * std::max(2, iColB);
    double* pdblW3 = pdblW2 + iSizeP;
    double* pdblW4 = pdblW3 + iSizeP;
    double* pdblW5 = pdblW4 + iColB;

    // placement de poles
    int iErr = 0;
    C2F(polmc)( &iSizeP, &iColB, &iSizeP, &iColB, pDblA->get(), pDblB->get(), pdblG, pdblPReal, pdblPImg,
                pdblZ, &inc, piW, &iErr, pdblW, pdblW + iColB, pdblW1, pdblW2, pdblW3, pdblW4, pdblW5);

    if (iErr)
    {
        Scierror(999, _("%s: Uncontrollable system.\n"), "ppol");
        delete[] pdblZ;
        delete[] pdblW;
        delete[] piW;
        if (isDeletable)
        {
            delete[] pdblPImg;
        }
        delete pDblA;
        delete pDblB;
        return types::Function::Error;
    }

    // free memory
    delete[] pdblZ;
    delete[] pdblW;
    delete[] piW;
    if (isDeletable)
    {
        delete[] pdblPImg;
    }
    delete pDblA;
    delete pDblB;

    /*** retrun output arguments ***/
    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
