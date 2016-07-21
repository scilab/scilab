/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bernard HUGUENEY
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
#include "sparse.hxx"
#include "gsort.hxx"

extern "C"
{
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
}

types::Function::ReturnValue sci_spget(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pValues      = NULL;
    types::Double* pCoordSorted = NULL;
    types::Bool* pBools         = NULL;

    int iRows           = 0;
    int iCols           = 0;
    bool bComplex       = false;
    int nonZeros        = 0;
    int* pRows       = NULL;
    double* pNonZeroR   = NULL;
    double* pNonZeroI   = NULL;

    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "spget", 1);
        return types::Function::Error;
    }

    if (in[0]->isSparse())
    {
        types::Sparse* sp = in[0]->getAs<types::Sparse>();
        nonZeros = static_cast<int>(sp->nonZeros());

        if (nonZeros == 0)
        {
            out.push_back(types::Double::Empty());
            if (_iRetCount > 1)
            {
                out.push_back(types::Double::Empty());
            }

            if (_iRetCount > 2)
            {
                types::Double* pSize = new types::Double(1, 2);
                pSize->set(0, sp->getRows());
                pSize->set(1, sp->getCols());
                out.push_back(pSize);
            }

            return types::Function::OK;
        }

        pRows = new int[nonZeros * 2];
        sp->outputRowCol(pRows);

        if (_iRetCount > 1)
        {
            pNonZeroR = new double[nonZeros];
            pNonZeroI = new double[nonZeros];
            sp->outputValues(pNonZeroR, pNonZeroI);
        }

        iRows = sp->getRows();
        iCols = sp->getCols();
        bComplex = sp->isComplex();

        pValues = new types::Double(nonZeros, 1, bComplex);

    }
    else if (in[0]->isSparseBool())
    {
        types::SparseBool* sp = in[0]->getAs<types::SparseBool>();
        nonZeros = static_cast<int>(sp->nbTrue());

        if (nonZeros == 0)
        {
            out.push_back(types::Double::Empty());
            if (_iRetCount > 1)
            {
                out.push_back(types::Double::Empty());
            }

            if (_iRetCount > 2)
            {
                types::Double* pSize = new types::Double(1, 2);
                pSize->set(0, sp->getRows());
                pSize->set(1, sp->getCols());
                out.push_back(pSize);
            }

            return types::Function::OK;
        }


        pRows = new int[nonZeros * 2];
        sp->outputRowCol(pRows);

        iRows = sp->getRows();
        iCols = sp->getCols();

        pBools = new types::Bool(nonZeros, 1);
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: sparse matrix expected.\n"), "spget", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 3)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d to %d expected.\n"), "spget", 1, 3);
        delete[] pRows;
        delete[] pNonZeroR;
        delete[] pNonZeroI;
        if (pBools)
        {
            pBools->killMe();
        }
        if (pValues)
        {
            pValues->killMe();
        }
        return types::Function::Error;
    }

    types::Double* pCoord = new types::Double(nonZeros, 2);
    pCoord->setInt(pRows);

    if (_iRetCount > 1)
    {
        if (pBools)
        {
            pCoordSorted = gsort(pCoord, NULL, L"i", L"lr");
            for (int i = 0 ; i < nonZeros ; i++)
            {
                pBools->set(i, true);
            }
        }
        else
        {
            types::Double* pIndex = new types::Double(nonZeros, 1);
            pCoordSorted = gsort(pCoord, pIndex, L"i", L"lr");
            if (pValues->isComplex())
            {
                for (int i = 0 ; i < nonZeros ; i++)
                {
                    pValues->set(i, pNonZeroR[(int)pIndex->get(i) - 1]);
                    pValues->setImg(i, pNonZeroI[(int)pIndex->get(i) - 1]);
                }
            }
            else
            {
                for (int i = 0 ; i < nonZeros ; i++)
                {
                    pValues->set(i, pNonZeroR[(int)pIndex->get(i) - 1]);
                }
            }

            delete[] pNonZeroR;
            delete[] pNonZeroI;
            delete pIndex;
        }
    }
    else
    {
        pCoordSorted = gsort(pCoord, NULL, L"i", L"lr");
    }

    out.push_back(pCoordSorted);
    if (_iRetCount > 1)
    {
        if (pBools)
        {
            out.push_back(pBools);
        }
        else
        {
            out.push_back(pValues);
        }
    }
    else
    {
        if (pBools)
        {
            pBools->killMe();
        }

        if (pValues)
        {
            pValues->killMe();
        }
    }

    if (_iRetCount > 2)
    {
        types::Double* pSize = new types::Double(1, 2);
        pSize->set(0, iRows);
        pSize->set(1, iCols);
        out.push_back(pSize);
    }

    delete pCoord;
    delete[] pRows;
    return types::Function::OK;
}
