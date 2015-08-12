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

/* desc : This function convert Scilab string to a vector of ascii code  */
/*        or vector of ascii code to Scilab strings.                     */
/*        If  txt  is a matrix of string,  ascii(txt)  is equivalent to  */
/*          ascii(strcat(txt))                                           */
/*-----------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "gw_string.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "getfastcode.h"
#include "warningmode.h"
#include "sciprint.h"
/*----------------------------------------------------------------------------*/
static int asciiStrings(char *fname);
static int asciiMatrix(char *fname);
static int asciiIntMatrix(char *fname);
static int asciiOthers(char *fname);
/*----------------------------------------------------------------------------*/
#define ASCII_MIN 0
#define ASCII_MAX 255
/*----------------------------------------------------------------------------*/
int sci_ascii(char *fname, unsigned long fname_len)
{
    CheckRhs(1, 1);
    CheckLhs(0, 1);

    switch ( GetType(1))
    {
        case sci_strings:
            asciiStrings(fname);
            break;

        case sci_matrix :
            asciiMatrix(fname);
            break;

        case sci_ints :
            asciiIntMatrix(fname);
            break;

        default:
            asciiOthers(fname);
            break;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int asciiStrings(char *fname)
{

    BOOL is_a_reference_on_stack = FALSE; /* variable is a reference on stack */
    int i = 0, j = 0;
    int nbr_characters  = 0;
    int  l = 0, il = 0, lr = 0;
    int ilr = 0;

    il = iadr( C2F(vstk).lstk[Top - 1] );

    ilr = il;
    if (*istk(il) < 0)
    {
        il = iadr(*istk(il + 1));
    }
    /* check if parameter is a reference */
    is_a_reference_on_stack = (il != ilr);

    /* find number of characters */
    nbr_characters = *istk(il + 4 + *istk(il + 1) **istk(il + 2)) - 1;

    l = il + 5 + *istk(il + 1) **istk(il + 2);

    if (is_a_reference_on_stack)
    {
        /* it is a reference on stack : txt = 'Scilab' ; ascii(txt) */
        j = ilr + 4;
        Err = j / 2 + 1 + nbr_characters - C2F(vstk).lstk[Bot - 1];
        if (Err > 0)
        {
            /* stacksize exceeded */
            SciError(17);
            return 0;
        }
    }
    else
    {
        /* it is not a reference on stack  ascii('Scilab') */
        int one = 1;
        int lw = l + nbr_characters;

        j = lw + nbr_characters;
        Err = j / 2 + 1 - C2F(vstk).lstk[Bot - 1];
        if (Err > 0)
        {
            /* stacksize exceeded */
            SciError(17);
            return 0;
        }
        C2F(icopy)(&nbr_characters, istk(l), &one, istk(l + nbr_characters), &one);
        l = lw;
    }

    /* create output int matrix , see C2F(crematvar) stack1.c */
    *istk(ilr) = sci_matrix;
    *istk(ilr + 1) = 1;
    *istk(ilr + 2) = nbr_characters;
    *istk(ilr + 3) = 0;

    j = ilr + 4;
    lr = j / 2 + 1;

    /* put each value on stack */
    for (i = 0; i < nbr_characters; i++)
    {
        int scilab_code = *istk(l + i);
        *stk(lr + i) = convertScilabCodeToAsciiCode(scilab_code);
    }
    /* update Top of stack */
    C2F(vstk).lstk[Top] = lr + nbr_characters;

    return 0;

}
/*--------------------------------------------------------------------------*/
static int asciiMatrix(char *fname)
{
    int Row_Num = 0, Col_Num = 0, Stack_Pos = 0;
    int outIndex = 0 ;
    int len = 0;
    BOOL FirstWarning = TRUE;
    /*When input vector of int ascii codes  */
    GetRhsVar(1, MATRIX_OF_INTEGER_DATATYPE, &Row_Num, &Col_Num, &Stack_Pos);
    len = Row_Num * Col_Num ;

    if (len != 0)
    {
        int x = 0;
        int one = 1;
        int *Input_IntMatrix = NULL;
        char *Output_StringMatrix = NULL;

        Input_IntMatrix = istk(Stack_Pos);

        outIndex = 0 ;
        CreateVar(Rhs + 1, STRING_DATATYPE, &len, &one, &outIndex);
        Output_StringMatrix = cstk(outIndex);
        for (x = 0; x < len; x++)
        {
            if ( FirstWarning )
            {
                if ( (Input_IntMatrix[x] < ASCII_MIN) || (Input_IntMatrix[x] > ASCII_MAX) )
                {
                    if (getWarningMode())
                    {
                        sciprint(_("WARNING : \n"));
                        sciprint(_("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"), fname, 1, ASCII_MIN, ASCII_MAX);
                    }
                    FirstWarning = FALSE;
                }
            }

            if (isascii(Input_IntMatrix[x]))
            {
                Output_StringMatrix[x] = (char)toascii(Input_IntMatrix[x]);
            }
            else
            {
                Output_StringMatrix[x] = (char)Input_IntMatrix[x];
            }
        }
        Output_StringMatrix[len] = '\0';
    }
    else
    {
#define EMPTY_STR ""
        int one    = 1 ;
        int lenStr   = (int)strlen(EMPTY_STR);
        outIndex = 0 ;

        CreateVar(Rhs + 1, STRING_DATATYPE, &lenStr, &one, &outIndex);
        strcpy(cstk(outIndex), EMPTY_STR);
    }

    LhsVar(1) = Rhs + 1 ;
    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
static int asciiIntMatrix(char *fname)
{
    int Row_Num = 0, Col_Num = 0;
    int outIndex = 0 ;
    int len = 0;

    SciIntMat M;

    GetRhsVar(1, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &Row_Num, &Col_Num, &M);
    len = Row_Num * Col_Num ;

    if (len != 0)
    {
        int one        = 1;
        int ichar      = I_UCHAR;
        static int inc = 1;

        char *Output_StringMatrix = NULL;

        CreateVar(Rhs + 1, STRING_DATATYPE, &len, &one, &outIndex);
        Output_StringMatrix = cstk(outIndex);

        /* from intxx to char */
        C2F(tpconv)(&M.it, &ichar, &len, M.D, &inc, Output_StringMatrix, &inc);
        Output_StringMatrix[len] = '\0';
    }
    else
    {
#define EMPTY_STR ""
        int one    = 1 ;
        int lenStr = (int)strlen(EMPTY_STR);

        CreateVar(Rhs + 1, STRING_DATATYPE, &lenStr, &one, &outIndex);
        strcpy(cstk(outIndex), EMPTY_STR);
    }

    LhsVar(1) = Rhs + 1 ;
    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
static int asciiOthers(char *fname)
{
    Scierror(999, _("%s: Wrong type for argument #%d: Matrix of strings or Integer matrix expected.\n"), fname, 1);
    return 0;
}
/*--------------------------------------------------------------------------*/
