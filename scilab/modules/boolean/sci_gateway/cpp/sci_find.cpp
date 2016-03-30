/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - DIGITEO - Antoine ELIAS
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

#include "boolean_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "bool.hxx"
#include "sparse.hxx"
#include "overload.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

static void getCoordFromIndex(int _iIndex, int* _piIndexes, int* _piDims, int _iDims);
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_find(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iMax = -1;
    if (in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "find", 1, 2);
        return types::Function::Error;
    }

    if (in.size() == 2)
    {
        if (in[1]->isDouble() == false || in[1]->getAs<types::Double>()->isScalar() == false)
        {
            Scierror(999, _("%s:  Wrong type for input argument #%d: Scalar positive integer expected.\n"), "find", 2);
            return types::Function::Error;
        }

        iMax = (int)in[1]->getAs<types::Double>()->get()[0];
        if (iMax <= 0 && iMax != -1)
        {
            Scierror(999, _("%s:  Wrong type for input argument #%d: Scalar positive integer expected.\n"), "find", 2);
            return types::Function::Error;
        }

    }

    int* piIndex = 0;
    int iValues = 0;

    if (in[0]->isGenericType() == false)
    {
        //call overload for other types
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_find";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    types::GenericType* pGT = in[0]->getAs<types::GenericType>();
    piIndex = new int[pGT->getSize()];

    if (in[0]->isBool())
    {
        types::Bool* pB = in[0]->getAs<types::Bool>();
        int size = pB->getSize();
        int* p = pB->get();
        iMax = iMax == -1 ? size : std::min(iMax, size);
        for (int i = 0 ; i < size && iValues < iMax ; i++)
        {
            if (p[i])
            {
                piIndex[iValues] = i;
                iValues++;
            }
        }
    }
    else if (in[0]->isDouble())
    {
        types::Double* pD = in[0]->getAs<types::Double>();
        int size = pD->getSize();
        double* p = pD->get();
        iMax = iMax == -1 ? size : std::min(iMax, size);
        for (int i = 0; i < size && iValues < iMax; i++)
        {
            if (p[i])
            {
                piIndex[iValues] = i;
                iValues++;
            }
        }
    }
    else if (in[0]->isSparse())
    {
        types::Sparse* pSP = in[0]->getAs<types::Sparse>();
        int iNNZ = (int)pSP->nonZeros();
        int iRows = pSP->getRows();
        int* pRows = new int[iNNZ * 2];
        pSP->outputRowCol(pRows);
        int *pCols = pRows + iNNZ;
        iMax = iMax == -1 ? iNNZ : std::min(iMax, iNNZ);

        for (int i = 0; i < iNNZ && iValues < iMax; i++)
        {
            piIndex[iValues] = (pCols[i] - 1) * iRows + (pRows[i] - 1);
            iValues++;
        }

        delete[] pRows;
    }
    else if (in[0]->isSparseBool())
    {
        types::SparseBool* pSB = in[0]->getAs<types::SparseBool>();
        int iNNZ = (int)pSB->nbTrue();
        int iRows = pSB->getRows();

        int* pRows = new int[iNNZ * 2];
        pSB->outputRowCol(pRows);
        int* pCols = pRows + iNNZ;

        iMax = iMax == -1 ? iNNZ : std::min(iMax, iNNZ);
        for (int i = 0; i < iNNZ && iValues < iMax; i++)
        {
            piIndex[iValues] = (pCols[i] - 1) * iRows + (pRows[i] - 1);
            iValues++;
        }

        delete[] pRows;
    }
    else
    {
        delete[] piIndex;

        //call overload for other types
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_find";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    if (iValues == 0)
    {
        for (int i = 0 ; i < _iRetCount ; i++)
        {
            out.push_back(types::Double::Empty());
        }
    }
    else
    {
        if (_iRetCount == 1)
        {
            types::Double* dbl = new types::Double(1, iValues);
            double* p = dbl->get();

            for (int i = 0; i < iValues; ++i)
            {
                p[i] = static_cast<double>(piIndex[i]) + 1;
            }

            delete[] piIndex;
            out.push_back(dbl);
            return types::Function::OK;
        }

        int* piRefDims = pGT->getDimsArray();
        int iRefDims = pGT->getDims();

        int* piDims = new int[_iRetCount];
        int iDims = _iRetCount;

        if (iDims == iRefDims)
        {
            for (int i = 0 ; i < iRefDims ; i++)
            {
                piDims[i] = piRefDims[i];
            }
        }
        else if (iDims > iRefDims)
        {
            for (int i = 0 ; i < iRefDims ; i++)
            {
                piDims[i] = piRefDims[i];
            }

            for (int i = iRefDims ; i < iDims ; i++)
            {
                piDims[i] = 1;
            }
        }
        else //iDims < iRefDims
        {
            for (int i = 0 ; i < iDims - 1 ; i++)
            {
                piDims[i] = piRefDims[i];
            }

            piDims[iDims - 1] = 1;
            for (int i = iDims - 1 ; i < iRefDims ; i++)
            {
                piDims[iDims - 1] *= piRefDims[i];
            }
        }

        int** piCoord = new int*[iValues];
        for (int i = 0 ; i < iValues ; i++)
        {
            piCoord[i] = new int[_iRetCount];
        }

        for (int i = 0 ; i < iValues ; i++)
        {
            getCoordFromIndex(piIndex[i], piCoord[i], piDims, iDims);
        }

        for (int i = 0 ; i < _iRetCount ; i++)
        {
            types::Double* pOut = new types::Double(1, iValues);
            for (int j = 0 ; j < iValues ; j++)
            {
                pOut->set(j, piCoord[j][i] + 1);
            }
            out.push_back(pOut);
        }

        delete[] piDims;
        for (int i = 0 ; i < iValues ; i++)
        {
            delete[] piCoord[i];
        }
        delete[] piCoord;
    }

    delete[] piIndex;
    return types::Function::OK;
}

static void getCoordFromIndex(int _iIndex, int* _piIndexes, int* _piDims, int _iDims)
{
    int iMul = 1;
    for (int i = 0 ; i < _iDims ; i++)
    {
        _piIndexes[i] = (int)(_iIndex / iMul) % _piDims[i];
        iMul *= _piDims[i];
    }
}
/*--------------------------------------------------------------------------*/
