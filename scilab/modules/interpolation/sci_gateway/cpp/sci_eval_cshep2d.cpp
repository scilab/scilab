/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include "interpolation_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "tlist.hxx"
#include "int32.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "interpolation_functions.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_eval_cshep2d(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    // input
    types::Double* pDblXp       = NULL;
    types::Double* pDblYp       = NULL;
    types::TList* pTListCoef    = NULL;
    types::Double* pDblCoef     = NULL;
    types::Int32* pInt32Cell    = NULL;
    types::Int32* pInt32Next    = NULL;
    types::Double* pDblGrid     = NULL;
    types::Double* pDblRMax     = NULL;
    types::Double* pDblRW       = NULL;
    types::Double* pDblA        = NULL;

    // output
    types::Double* pDblF        = NULL;
    types::Double* pDblDfdx     = NULL;
    types::Double* pDblDfdy     = NULL;
    types::Double* pDblDffdxx   = NULL;
    types::Double* pDblDffdxy   = NULL;
    types::Double* pDblDffdyy   = NULL;

    // *** check the minimal number of input args. ***
    if (in.size() != 3)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"eval_cshep2d", 3);
        return types::Function::Error;
    }

    // *** check number of output args according the methode. ***
    if (_iRetCount != 3 && _iRetCount != 6 && _iRetCount > 1)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d, %d or %d expected.\n"), L"eval_cshep2d", 1, 3, 6);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    //xp
    if (in[0]->isDouble() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A matrix expected.\n"), L"eval_cshep2d", 1);
        return types::Function::Error;
    }

    pDblXp = in[0]->getAs<types::Double>();

    //yp
    if (in[1]->isDouble() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A matrix expected.\n"), L"eval_cshep2d", 2);
        return types::Function::Error;
    }

    pDblYp = in[1]->getAs<types::Double>();

    if (pDblYp->getRows() != pDblXp->getRows() || pDblYp->getCols() != pDblXp->getCols())
    {
        ScierrorW(999, _W("%ls: Wrong size for input arguments #%d ans #%d: Same size expected.\n"), L"eval_cshep2d", 1, 2);
        return types::Function::Error;
    }

    //coef
    if (in[2]->isTList() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A tlist of type %ls expected.\n"), L"eval_cshep2d", 3, L"cshep2d");
        return types::Function::Error;
    }

    pTListCoef = in[2]->getAs<types::TList>();

    if (pTListCoef->getTypeStr() != L"cshep2d")
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A %ls tlist expected.\n"), L"interp3d", 3, L"cshep2d");
        return types::Function::Error;
    }

    pDblCoef    = pTListCoef->get(L"xyz")->getAs<types::Double>();
    pInt32Cell  = pTListCoef->get(L"lcell")->getAs<types::Int32>();
    pInt32Next  = pTListCoef->get(L"lnext")->getAs<types::Int32>();
    pDblGrid    = pTListCoef->get(L"grdim")->getAs<types::Double>();
    pDblRMax    = pTListCoef->get(L"rmax")->getAs<types::Double>();
    pDblRW      = pTListCoef->get(L"rw")->getAs<types::Double>();
    pDblA       = pTListCoef->get(L"a")->getAs<types::Double>();

    // *** Perform operation. ***
    int ier         = 0;
    int nr          = pInt32Cell->getRows();
    int rows        = pDblCoef->getRows();
    int sizeOfXp    = pDblXp->getSize();

    pDblF = new types::Double(pDblXp->getRows(), pDblXp->getCols());

    if (_iRetCount == 1)
    {
        for (int i = 0; i < sizeOfXp; i++)
        {
            double ret = C2F(cs2val)(pDblXp->get() + i, pDblYp->get() + i, &rows, pDblCoef->get(),
                                     pDblCoef->get() + rows, pDblCoef->get() + (2 * rows), &nr,
                                     pInt32Cell->get(), pInt32Next->get(), pDblGrid->get(),
                                     pDblGrid->get() + 1, pDblGrid->get() + 2, pDblGrid->get() + 3,
                                     pDblRMax->get(), pDblRW->get(), pDblA->get());

            pDblF->set(i, ret);
        }
    }
    else// if(_iRetCount > 2)
    {
        pDblDfdx   = new types::Double(pDblXp->getRows(), pDblXp->getCols());
        pDblDfdy   = new types::Double(pDblXp->getRows(), pDblXp->getCols());
        if (_iRetCount == 3)
        {
            for (int i = 0; i < sizeOfXp; i++)
            {
                C2F(cs2grd)(pDblXp->get() + i, pDblYp->get() + i, &rows, pDblCoef->get(),
                            pDblCoef->get() + rows, pDblCoef->get() + (2 * rows), &nr,
                            pInt32Cell->get(), pInt32Next->get(), pDblGrid->get(),
                            pDblGrid->get() + 1, pDblGrid->get() + 2, pDblGrid->get() + 3,
                            pDblRMax->get(), pDblRW->get(), pDblA->get(), pDblF->get() + i,
                            pDblDfdx->get() + i, pDblDfdy->get() + i, &ier);
            }
        }
        else // == 6
        {
            pDblDffdxx = new types::Double(pDblXp->getRows(), pDblXp->getCols());
            pDblDffdxy = new types::Double(pDblXp->getRows(), pDblXp->getCols());
            pDblDffdyy = new types::Double(pDblXp->getRows(), pDblXp->getCols());

            for (int i = 0; i < sizeOfXp; i++)
            {
                C2F(cs2hes)(pDblXp->get() + i, pDblYp->get() + i, &rows, pDblCoef->get(),
                            pDblCoef->get() + rows, pDblCoef->get() + (2 * rows), &nr,
                            pInt32Cell->get(), pInt32Next->get(), pDblGrid->get(),
                            pDblGrid->get() + 1, pDblGrid->get() + 2, pDblGrid->get() + 3,
                            pDblRMax->get(), pDblRW->get(), pDblA->get(), pDblF->get() + i,
                            pDblDfdx->get() + i, pDblDfdy->get() + i, pDblDffdxx->get() + i,
                            pDblDffdxy->get() + i, pDblDffdyy->get() + i, &ier);

            }
        }
    }

    // *** Return result in Scilab. ***
    out.push_back(pDblF);

    if (_iRetCount > 2)
    {
        out.push_back(pDblDfdx);
        out.push_back(pDblDfdy);
    }

    if (_iRetCount == 6)
    {
        out.push_back(pDblDffdxx);
        out.push_back(pDblDffdyy);
        out.push_back(pDblDffdxy);
    }

    return types::Function::OK;
}

