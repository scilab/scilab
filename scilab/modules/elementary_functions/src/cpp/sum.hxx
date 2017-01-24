/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
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

#ifndef __SUM_H__
#define __SUM_H__

#include "double.hxx"
#include "polynom.hxx"

extern "C"
{
#include "dynlib_elementary_functions.h"
}

ELEMENTARY_FUNCTIONS_IMPEXP types::Double*  sum(types::Double*  pIn, int iOrientation);
ELEMENTARY_FUNCTIONS_IMPEXP types::Polynom* sum(types::Polynom* pIn, int iOrientation);

template <class T>
T* sum(T* pIn, int iOrientation)
{
    T* pOut = NULL;
    typename T::type* pR = pIn->get();

    if (iOrientation == 0) // all
    {
        typename T::type r = 0;

        for (int i = 0; i < pIn->getSize(); i++)
        {
            r += pR[i];
        }

        pOut = new T(r);
    }
    else // sum on one dimension
    {
        // create output dimensions
        int iDims = pIn->getDims();
        int* piDims = new int[iDims];

        for (int i = 0; i < iDims; i++)
        {
            piDims[i] = pIn->getDimsArray()[i];
        }

        piDims[iOrientation - 1] = 1;

        // create output variable
        pOut = new T(iDims, piDims);
        typename T::type* pROut = pOut->get();

        // init output
        for (int i = 0; i < pOut->getSize(); ++i)
        {
            pROut[i] = 0;
        }

        delete[] piDims;
        piDims = NULL;

        // perform operations
        int* piIndex = new int[iDims];
        for (int i = 0; i < pIn->getSize(); i++)
        {
            //get array of dim
            pIn->getIndexes(i, piIndex);

            //convert indexes for result
            piIndex[iOrientation - 1] = 0;
            int iIndex = pOut->getIndex(piIndex);
            pROut[iIndex] += pR[i];
        }

        delete[] piIndex;
        piIndex = NULL;
    }

    return pOut;
}

#endif /* __SUM_H__ */
