/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bernard HUGUENEY
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_gw.hxx"
#include "function.hxx"
#include "sparse.hxx"

extern "C"
{
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
}

using namespace types;
#define CONST

Function::ReturnValue sci_spget(typed_list &in, int nbRes, typed_list &out)
{
    types::Sparse *pRetVal = NULL;
    if(in.size() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"spget", 1);
        return Function::Error;
    }

    if(in[0]->isSparse() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: sparse matrix expected.\n"), L"spget", 1);
        return Function::Error;
    }

    if(nbRes > 3)
    {
        ScierrorW(999, _W("%ls: Wrong number of output arguments: %d to %d expected.\n"), L"spget", 1, 3);
        return Function::Error;
    }

    types::Sparse* sp = in[0]->getAs<Sparse>();
    int const nonZeros = static_cast<int>(sp->nonZeros());

    Double* pCoord = new Double(nonZeros, 2);
    Double* pValues = NULL;
    if(nbRes > 1)
    {
        pValues = new Double(nonZeros, 1, sp->isComplex());
    }

    double* pRows = new double[nonZeros * 2];
    sp->outputRowCol(pRows);
    double* pCols = pRows + nonZeros;

    double* pNonZeroR = new double[nonZeros];
    double* pNonZeroI = new double[nonZeros];
    sp->outputValues(pNonZeroR, pNonZeroI);

    int iIndex = 0;
    for(int iCol = 0 ; iCol < sp->getRows() ; iCol++)
    {
        for(int i = 0 ; i < nonZeros ; i++)
        {
            if(pRows[i] == iCol + 1)
            {//good row
                pCoord->set(iIndex, 0, pRows[i]);
                pCoord->set(iIndex, 1, pCols[i]);
                if(pValues)
                {
                    pValues->set(iIndex, pNonZeroR[i]);
                    if(pValues->isComplex())
                    {
                        pValues->setImg(iIndex, pNonZeroI[i]);
                    }
                }
                iIndex++;
            }
        }
    }

    out.push_back(pCoord);
    if(nbRes > 1)
    {
        out.push_back(pValues);
    }

    if(nbRes > 2)
    {
        Double* pSize = new Double(1, 2);
        pSize->set(0, sp->getRows());
        pSize->set(1, sp->getCols());
        out.push_back(pSize);
    }

    delete[] pRows;
    delete[] pNonZeroR;
    delete[] pNonZeroI;
    return Function::OK;
}
