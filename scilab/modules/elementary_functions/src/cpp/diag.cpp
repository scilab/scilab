/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012-2014 - DIGITEO - Cedric Delamarre
 * Copyright (C) 2014 - Scilab Enterprises - Sylvain GENIN
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

#include "diag.hxx"

types::InternalType* diag(types::Polynom* pIn,  int iStartPos)
{
    types::Polynom* pPolyOut    = NULL;
    types::SinglePoly* pSP      = NULL;

    int iRows = pIn->getRows();
    int iCols = pIn->getCols();

    int iSize       = 0;
    int iStartRow   = 0;
    int iStartCol   = 0;
    int iPos        = 0;
    int iRank       = 0;

    double* pdRData = 0;
    double* pdIData = 0;

    if (iRows != 1 && iCols != 1) // pIn is a matrix
    {
        if (iStartPos < 0)
        {
            iSize = std::max(0, std::min(iRows + iStartPos, iCols));
            iStartRow = -iStartPos;
        }
        else
        {
            iSize = std::max(0, std::min(iRows, iCols - iStartPos));
            iStartCol = iStartPos;
        }

        if (iSize)
        {
            pPolyOut = new types::Polynom(pIn->getVariableName(), iSize, 1);
        }
        else
        {
            return types::Double::Empty();
        }

        for (int i = 0; i < iSize; i++)
        {
            iPos = (i + iStartCol) * iRows + (i + iStartRow);
            pPolyOut->set(i, pIn->get(iPos));
        }
    }
    else // pIn is a vector
    {
        int iSizeOfVector = std::max(iRows, iCols);
        if (iStartPos < 0)
        {
            iSize = iSizeOfVector - iStartPos;
            iStartRow = -iStartPos;
        }
        else
        {
            iSize = iSizeOfVector + iStartPos;
            iStartCol = iStartPos;
        }

        int* piRanks = new int[iSize * iSize];
        memset(piRanks, 0x00, iSize * iSize * sizeof(int));
        pPolyOut = new types::Polynom(pIn->getVariableName(), iSize, iSize, piRanks);
        delete[] piRanks;
        pPolyOut->setZeros();
        pPolyOut->setComplex(pIn->isComplex());

        for (int i = 0; i < iSizeOfVector; i++)
        {
            iPos = (i + iStartCol) * iSize + (i + iStartRow);
            pPolyOut->set(iPos, pIn->get(i));
        }
    }

    return pPolyOut;
}

types::InternalType* diag(types::String* pIn,  int iStartPos)
{
    types::String* pStrOut = NULL;

    int iRows = pIn->getRows();
    int iCols = pIn->getCols();

    int iSize       = 0;
    int iStartRow   = 0;
    int iStartCol   = 0;
    int iPos        = 0;

    if (iRows != 1 && iCols != 1) // pIn is a matrix
    {
        if (iStartPos < 0)
        {
            iSize = std::max(0, std::min(iRows + iStartPos, iCols));
            iStartRow = -iStartPos;
        }
        else
        {
            iSize = std::max(0, std::min(iRows, iCols - iStartPos));
            iStartCol = iStartPos;
        }

        if (iSize)
        {
            pStrOut = new types::String(iSize, 1);
        }
        else
        {
            return types::Double::Empty();
        }

        for (int i = 0; i < iSize; i++)
        {
            iPos = (i + iStartCol) * iRows + (i + iStartRow);
            pStrOut->set(i, pIn->get(iPos));
        }
    }
    else // pIn is a vector
    {
        int iSizeOfVector = std::max(iRows, iCols);
        if (iStartPos < 0)
        {
            iSize = iSizeOfVector - iStartPos;
            iStartRow = -iStartPos;
        }
        else
        {
            iSize = iSizeOfVector + iStartPos;
            iStartCol = iStartPos;
        }

        pStrOut = new types::String(iSize, iSize);

        for (int i = 0; i < iSize * iSize; i++)
        {
            pStrOut->set(i, L"");
        }

        for (int i = 0; i < iSizeOfVector; i++)
        {
            iPos = (i + iStartCol) * iSize + (i + iStartRow);
            pStrOut->set(iPos, pIn->get(i));
        }
    }

    return pStrOut;
}
