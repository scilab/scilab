/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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

#include "interpolation_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "tlist.hxx"
#include "int.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "interpolation_functions.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_cshep2d(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    //input
    types::Double* pDblXYZ = NULL;

    //output
    types::TList* pTListCoef = NULL;

    const wchar_t * wcsStr[] = {L"cshep2d", L"xyz", L"lcell", L"lnext", L"grdim", L"rmax", L"rw", L"a"};

    // *** check the minimal number of input args. ***
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "cshep2d", 1);
        return types::Function::Error;
    }

    // *** check number of output args according the methode. ***
    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "cshep2d", 1);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    //xyz
    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "cshep2d", 1);
        return types::Function::Error;
    }

    pDblXYZ = in[0]->getAs<types::Double>()->clone()->getAs<types::Double>();

    if (pDblXYZ->isComplex())
    {
        Scierror(999, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), "cshep2d", 1);
        return types::Function::Error;
    }

    if (pDblXYZ->getCols() != 3 || pDblXYZ->getRows() < 10)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : A matrix of size n x 3 expected.(n >= 10)\n"), "cshep2d", 1);
        return types::Function::Error;
    }

    // *** Perform operation. ***
    // choise for nc (can be a futur optionnal parameter)
    int nc = std::min(17, pDblXYZ->getRows() - 1);
    // choise for nw
    int nw = std::min(30, pDblXYZ->getRows() - 1);
    // choise for nr (grid nr x nr)
    int nr = (int) sqrt(pDblXYZ->getRows() / 3.0); // comme n >= 10 nr >= 1

    pTListCoef = new types::TList();

    types::String* pStr = new types::String(1, 8);
    pStr->set(wcsStr);

    types::Int32* pInt32Cell    = new types::Int32(nr, nr);
    types::Int32* pInt32Next    = new types::Int32(1, pDblXYZ->getRows());
    types::Double* pDblGrid     = new types::Double(1, 4);
    types::Double* pDblRMax     = new types::Double(1, 1);
    types::Double* pDblRW       = new types::Double(1, pDblXYZ->getRows());
    types::Double* pDblA        = new types::Double(9, pDblXYZ->getRows());

    int ier = 0;
    int rows = pDblXYZ->getRows();

    C2F(cshep2)(&rows, pDblXYZ->get(), pDblXYZ->get() + rows, pDblXYZ->get() + (2 * rows),
                &nc, &nw, &nr, pInt32Cell->get(), pInt32Next->get(), pDblGrid->get(),
                pDblGrid->get() + 1, pDblGrid->get() + 2, pDblGrid->get() + 3,
                pDblRMax->get(), pDblRW->get(), pDblA->get(), &ier);

    if (ier != 0)
    {
        Scierror(999, _("%s: Duplicate nodes or all nodes colinears (ier = %d).\n"), "cshep2d", ier);
        delete pTListCoef;
        delete pStr;
        delete pInt32Cell;
        delete pInt32Next;
        delete pDblGrid;
        delete pDblRMax;
        delete pDblRW;
        delete pDblA;
        return types::Function::Error;
    }


    // *** Return result in Scilab. ***
    pTListCoef->append(pStr);
    pTListCoef->append(pDblXYZ);
    pTListCoef->append(pInt32Cell);
    pTListCoef->append(pInt32Next);
    pTListCoef->append(pDblGrid);
    pTListCoef->append(pDblRMax);
    pTListCoef->append(pDblRW);
    pTListCoef->append(pDblA);

    out.push_back(pTListCoef);

    return types::Function::OK;
}

