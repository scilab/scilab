/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#include <string.h>
#include "convertbase.h"
/*--------------------------------------------------------------------------*/ 
#define char_a 'a'
#define char_A 'A'
#define char_z 'z'
#define char_Z 'Z'
#define char_zero '0'
#define char_nine '9'
/*--------------------------------------------------------------------------*/ 
double convertBase2Dec(const char *pStr, int numberbase, int *ierr)
{
    double result = 0.;
    *ierr = 1;

    if (pStr)
    {
        size_t i = 0;
        size_t len = strlen(pStr);
        for (i = 0; i < len; i++)
        {
            if ((pStr[i] >= char_zero) && (pStr[i] <= char_nine))
            {
                result = (numberbase * result) + (int)pStr[i] - char_zero;
            }
            else if ((pStr[i] >= char_A) && (pStr[i] <= char_Z))
            {
                result = (numberbase * result) + (int)pStr[i] - char_A + 10;
            }
            else if ((pStr[i] >= char_a) && (pStr[i] <= char_z))
            {
                result = (numberbase * result) + (int)pStr[i] - char_a + 10;
            }
            else
            {
                *ierr = 1;
                return 0;
            }
        }
        *ierr = 0;
    }
    return result;
}
/*--------------------------------------------------------------------------*/
