/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "clean.hxx"

extern "C"
{
#include "basic_functions.h"
}

/*--------------------------- Double ---------------------------------------*/
void clean(double* pdblReal, double* pdblImg, int iSize, double dEpsA, double dEpsR)
{
    if (pdblImg)
    {
        double dNorm = wasums(iSize, pdblReal, pdblImg);
        double dEps = Max(dEpsA, dEpsR * dNorm);
        for (int i = 0 ; i < iSize ; i++)
        {
            if (dabss(pdblImg[i]) <= dEps)
            {
                pdblImg[i] = 0;
            }

            if (dabss(pdblReal[i]) <= dEps)
            {
                pdblReal[i] = 0;
            }
        }
    }
    else
    {
        int iOne = 1;
        double dNorm = C2F(dasum)(&iSize, pdblReal, &iOne);
        double dEps = Max(dEpsA, dEpsR * dNorm);
        for (int i = 0 ; i < iSize ; i++)
        {
            if (dabss(pdblReal[i]) <= dEps)
            {
                pdblReal[i] = 0;
            }
        }
    }
}
