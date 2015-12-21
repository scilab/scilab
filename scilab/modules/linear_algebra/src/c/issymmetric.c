
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Bruno Jofret <bruno.jofret@inria.fr>
 * Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*
**  -*- C -*-
**
** issymmetric.c
**
*/
#include "issymmetric.h"
/*
** Check whether or not a Matrix is Symmetric.
*/

/*
**   Two cases :
**   ~ Real Case :
**   --------------
**   - Check that symmetrics elements are strictly equals.
**
**   ~ Complex Case :
**   -----------------
**   - Check that symmetrics elements are conjugate.
**   - Check that diagonal's elements are Real.
*/


int isSymmetric(double* _pdblReal, double* _pdblImg, int _iIsComplex, int _iRows, int _iCols)
{
    int iDiag   = 0;
    int iRow    = 0;
    int iCol    = 0;

    // If the matrix is not Square, it can not be symmetric
    if (_iRows != _iCols)
    {
        return NOT_SYMMETRIC;
    }

    if (_iIsComplex)
    {
        // complex case

        //matrix must be an Hermitian matrix

        //Diag must be real
        for (iDiag = 0 ; iDiag < _iRows ; iDiag++)
        {
            int iPos = iDiag + iDiag * _iRows;
            if (_pdblImg[iPos] != 0)
            {
                return NOT_SYMMETRIC;
            }
        }

        //real part == real part and img part == -(img part)
        for (iRow = 0 ; iRow < _iRows; iRow++)
        {
            for (iCol = 0 ; iCol < iRow ; iCol++)
            {
                int iRef    = iRow + iCol * _iRows;
                int iCheck  = iCol + iRow * _iRows;

                if (_pdblReal[iRef] != _pdblReal[iCheck])
                {
                    //real part
                    return NOT_SYMMETRIC;
                }

                if (_pdblImg[iRef] != -(_pdblImg[iCheck]))
                {
                    //real part
                    return NOT_SYMMETRIC;
                }

            }
        }
    }
    else
    {
        //real case
        for (iRow = 0 ; iRow < _iRows; iRow++)
        {
            for (iCol = 0 ; iCol < iRow ; iCol++)
            {
                int iRef    = iRow + iCol * _iRows;
                int iCheck  = iCol + iRow * _iRows;

                if (_pdblReal[iRef] != _pdblReal[iCheck])
                {
                    //real part
                    return NOT_SYMMETRIC;
                }
            }
        }
    }

    return SYMMETRIC;
}
