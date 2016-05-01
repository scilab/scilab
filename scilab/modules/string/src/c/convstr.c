
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
 * Copyright (C) INRIA - Allan CORNET
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
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>
#include "convstr.h"
/*--------------------------------------------------------------------------*/
void convstr(wchar_t **Input_Matrix, wchar_t **Output_Matrix, char typ, int mn)
{
    int x = 0;
    for (x = 0 ; x < mn; x++)
    {
        int y = 0;
        size_t len_Input_Matrix = wcslen(Input_Matrix[x]);
        for (y = 0; y < len_Input_Matrix; y++)
        {
            /*To traverse every string in the string matrix */
            if ( (typ == UPPER) || (typ == UPPER_B) )
            {
                /*converts the matrix of strings  str-matrix into upper case */
                Output_Matrix[x][y] = (wchar_t) towupper(Input_Matrix[x][y]);
            }
            else if ( (typ == LOW) || (typ == LOW_B) )
            {
                /*converts the matrix of strings  str-matrix  into lower case */
                Output_Matrix[x][y] = (wchar_t) towlower(Input_Matrix[x][y]);
            }
        }
        Output_Matrix[x][y] = 0;
    }
}
/*--------------------------------------------------------------------------*/
