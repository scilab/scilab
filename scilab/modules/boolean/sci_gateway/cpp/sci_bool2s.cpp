/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Antoine ELIAS
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
#include "sparse.hxx"
#include "bool.hxx"
#include "overload.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_bool2s(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::InternalType* pOut = NULL;
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "bool2s", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble())
    {
        types::Double* pIn = in[0]->getAs<types::Double>();

        if (pIn->isComplex())
        {
            std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_bool2s";
            return Overload::call(wstFuncName, in, _iRetCount, out);
        }

        types::Double* pD = new types::Double(pIn->getDims(), pIn->getDimsArray());

        double* pdblIn  = pIn->get();
        double* pdblOut = pD->get();

        for (int i = 0 ; i < pIn->getSize() ; i++)
        {
            pdblOut[i] = static_cast<double>(pdblIn[i] != 0);
        }
        pOut = pD;
    }
    else if (in[0]->isBool())
    {
        types::Bool* pIn = in[0]->getAs<types::Bool>();
        types::Double* pD = new types::Double(pIn->getDims(), pIn->getDimsArray());

        int* piIn = pIn->get();
        double* pdblOut = pD->get();

        for (int i = 0 ; i < pIn->getSize() ; i++)
        {
            pdblOut[i] = static_cast<double>(piIn[i]);
        }
        pOut = pD;
    }
    else if (in[0]->isSparse())
    {
        types::Sparse* pSpIn = in[0]->getAs<types::Sparse>();

        if (pSpIn->isComplex())
        {
            std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_bool2s";
            return Overload::call(wstFuncName, in, _iRetCount, out);
        }

        types::Sparse* pSpOut = new types::Sparse(pSpIn->getRows(), pSpIn->getCols());

        int iNonZeros = static_cast<int>(pSpIn->nonZeros());

        //coords
        int* pRows = new int[iNonZeros * 2];
        pSpIn->outputRowCol(pRows);
        int* pCols = pRows + iNonZeros;

        //values
        double* pNonZeroR = new double[iNonZeros];
        double* pNonZeroI = new double[iNonZeros];
        pSpIn->outputValues(pNonZeroR, pNonZeroI);

        for (int i = 0; i < iNonZeros; i++)
        {
            pSpOut->set(pRows[i] - 1, pCols[i] - 1, static_cast<double>(pNonZeroR[i] != 0));
        }

        pOut = pSpOut;
        delete[] pNonZeroR;
        delete[] pNonZeroI;
        delete[] pRows;
    }
    else if (in[0]->isSparseBool())
    {
        types::SparseBool* pSpbIn = in[0]->getAs<types::SparseBool>();
        types::Sparse* pSpOut = new types::Sparse(pSpbIn ->getRows(), pSpbIn ->getCols());

        int iNonZeros = static_cast<int>(pSpbIn->nbTrue());

        //coords
        int* pRows = new int[iNonZeros * 2];
        pSpbIn->outputRowCol(pRows);
        int* pCols = pRows + iNonZeros;

        for (int i = 0; i < iNonZeros; i++)
        {
            pSpOut->set(pRows[i] - 1, pCols[i] - 1, 1);
        }

        pOut = pSpOut;
        delete[] pRows;
    }
    else
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_bool2s";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
