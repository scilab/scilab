/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/* desc : strips leading and trailing blanks (and tabs) of strings
txt=stripblanks(txt[,tabs])
Parameters
txt : string or matrix of strings
tabs : if TRUE then tabs are also stripped (default value is FALSE)*/
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_string.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "stripblanks.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
#include "localization.h"
/*----------------------------------------------------------------------------*/
int sci_stripblanks(char *fname, unsigned long fname_len)
{
    char **Input_String_Matrix_One = NULL;
    char **Output_String_Matrix = NULL;
    int numRow = 0;        /*@ The row number of the output string matrix*/
    int numCol = 0;        /*@ The col number of the output string matrix*/
    int m1 = 0, n1 = 0, mn = 0, i = 0;
    BOOL bREMOVE_TAB = FALSE; /* DEFAULT no TAB */
    int Type_One = VarType(1);

    CheckRhs(1, 2);
    CheckLhs(1, 1);

    if (Rhs == 2)
    {
        int Type_Two = VarType(2);

        if (Type_Two == sci_boolean)
        {
            int m2 = 0, n2 = 0, l2 = 0;
            GetRhsVar(2, MATRIX_OF_BOOLEAN_DATATYPE, &m2, &n2, &l2);
            bREMOVE_TAB = (BOOL) * istk(l2);
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 2);
            return 0;
        }
    }

    switch (Type_One)
    {
        case sci_matrix :
        {
            /* case stripblanks([]) */
            GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &Input_String_Matrix_One);
            if ( (m1 == 0) && (n1 == 0) )
            {
                int l = 0;
                CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l);
                LhsVar(1) = Rhs + 1 ;
                PutLhsVar();
                return 0;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"), fname, 1);
                return 0;
            }
        }
        break;
        case sci_strings :
        {
            GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &m1, &n1, &Input_String_Matrix_One);
            mn = m1 * n1;
        }
        break;
        default :
            Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"), fname, 1);
            return 0;
    }

    if (mn > 0)
    {
        Output_String_Matrix = (char**)MALLOC(sizeof(char*) * (mn));
    }
    else
    {
        Output_String_Matrix = NULL;
    }

    if (Output_String_Matrix == NULL)
    {
        freeArrayOfString(Input_String_Matrix_One, mn);
        Scierror(999, _("%s : No more memory.\n"), fname);
        return 0;
    }

    for (i = 0; i < mn ; i++)  /*@ To malloc a space the same as input string*/
    {
        Output_String_Matrix[i] = (char*)MALLOC(sizeof(char) * (strlen(Input_String_Matrix_One[i]) + 1));
        if (Output_String_Matrix[i] == NULL)
        {
            freeArrayOfString(Input_String_Matrix_One, mn);
            freeArrayOfString(Output_String_Matrix, i);
            Scierror(999, _("%s : No more memory.\n"), fname);
            return 0;
        }
    }

    /*@ The stripblank function*/
    stripblanks(Input_String_Matrix_One, Output_String_Matrix, mn, bREMOVE_TAB);

    freeArrayOfString(Input_String_Matrix_One, mn);

    /* put result on scilab stack */
    numRow   = m1;
    numCol   = n1;
    CreateVarFromPtr( Rhs + 1, MATRIX_OF_STRING_DATATYPE, &numRow, &numCol, Output_String_Matrix); /*@ Output*/

    /* free pointers */
    freeArrayOfString(Output_String_Matrix, mn);

    LhsVar(1) = Rhs + 1 ;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
