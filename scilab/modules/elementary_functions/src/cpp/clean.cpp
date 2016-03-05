/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
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
