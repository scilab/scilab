/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998 - ENPC - Jean-Philippe CHANCELIER
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
/* Written by Jean-Philippe Chancelier
 * Modified (restructuration and bug fix) by Allan Cornet                   */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "core_math.h"
#include "gsort.h"

#include "../../../string/includes/men_Sutils.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
#include "qsort.h"
#include "qsort-string.h"
#include "qsort-short.h"
#include "qsort-int.h"
#include "qsort-double.h"
#include "qsort-char.h"

/*--------------------------------------------------------------------------*/
/******************************************************
 * General sort routine for Scilab
 * xI is the transmitted table to sort ( if table is int )
 * xD is the transmitted table to sort ( if table is double )
 * ind is the int table to store the permutation
 *     (which is to be initialized and changed )
 * iflag == if 1 ind is to be computed if 0 ind is ignored
 * m,n : matrix size
 * type : the operation ( see the interface )
 * iord : 'i' or 'd' : increasind or decreasing sort
 ******************************************************/
int C2F(gsortd)(double *xD, int *ind, int *iflag, int *m, int *n, char *type, char *iord)
{

    switch ( type[0])
    {
        case 'r' :
            ColSortdouble(xD, ind, *iflag, *m, *n, iord[0]);
            break;
        case 'c' :
            RowSortdouble(xD, ind, *iflag, *m, *n, iord[0]);
            break;
        case 'l' :
            if ( type[1] == 'r' )
            {
                CNAME(LexiRow, double)(xD, ind, *iflag, *m, *n, iord[0]);
            }
            else
            {
                CNAME(LexiCol, double)(xD, ind, *iflag, *m, *n, iord[0]);
            }
            break;
        case 'g' :
        default :
            GlobalSortdouble(xD, ind, *iflag, *m, *n, iord[0]);
            break;
    }
    return(0);
}

int C2F(gsortint)(int *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord)
{

    switch ( type[0])
    {
        case 'r' :
            ColSortint(xI, ind, *iflag, *m, *n, iord[0]);
            break;
        case 'c' :
            RowSortint(xI, ind, *iflag, *m, *n, iord[0]);
            break;
        case 'l' :
            if ( type[1] == 'r' )
            {
                CNAME(LexiRow, int)(xI, ind, *iflag, *m, *n, iord[0]);
            }
            else
            {
                CNAME(LexiCol, int)(xI, ind, *iflag, *m, *n, iord[0]);
            }
            break;
        case 'g' :
        default :
            GlobalSortint(xI, ind, *iflag, *m, *n, iord[0]);
            break;
    }
    return(0);
}

int C2F(gsortuint)(unsigned int *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord)
{

    switch ( type[0])
    {
        case 'r' :
            ColSortuint(xI, ind, *iflag, *m, *n, iord[0]);
            break;
        case 'c' :
            RowSortuint(xI, ind, *iflag, *m, *n, iord[0]);
            break;
        case 'l' :
            if ( type[1] == 'r' )
            {
                CNAME(LexiRow, uint)(xI, ind, *iflag, *m, *n, iord[0]);
            }
            else
            {
                CNAME(LexiCol, uint)(xI, ind, *iflag, *m, *n, iord[0]);
            }
            break;
        case 'g' :
        default :
            GlobalSortuint(xI, ind, *iflag, *m, *n, iord[0]);
            break;
    }
    return(0);
}

int C2F(gsortshort)(short *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord)
{

    switch ( type[0])
    {
        case 'r' :
            ColSortshort(xI, ind, *iflag, *m, *n, iord[0]);
            break;
        case 'c' :
            RowSortshort(xI, ind, *iflag, *m, *n, iord[0]);
            break;
        case 'l' :
            if ( type[1] == 'r' )
            {
                CNAME(LexiRow, short)(xI, ind, *iflag, *m, *n, iord[0]);
            }
            else
            {
                CNAME(LexiCol, short)(xI, ind, *iflag, *m, *n, iord[0]);
            }
            break;
        case 'g' :
        default :
            GlobalSortshort(xI, ind, *iflag, *m, *n, iord[0]);
            break;
    }
    return(0);
}

int C2F(gsortushort)(unsigned short *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord)
{

    switch ( type[0])
    {
        case 'r' :
            ColSortushort(xI, ind, *iflag, *m, *n, iord[0]);
            break;
        case 'c' :
            RowSortushort(xI, ind, *iflag, *m, *n, iord[0]);
            break;
        case 'l' :
            if ( type[1] == 'r' )
            {
                CNAME(LexiRow, ushort)(xI, ind, *iflag, *m, *n, iord[0]);
            }
            else
            {
                CNAME(LexiCol, ushort)(xI, ind, *iflag, *m, *n, iord[0]);
            }
            break;
        case 'g' :
        default :
            GlobalSortushort(xI, ind, *iflag, *m, *n, iord[0]);
            break;
    }
    return(0);
}

int C2F(gsortchar)(char *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord)
{

    switch ( type[0])
    {
        case 'r' :
            ColSortchar(xI, ind, *iflag, *m, *n, iord[0]);
            break;
        case 'c' :
            RowSortchar(xI, ind, *iflag, *m, *n, iord[0]);
            break;
        case 'l' :
            if ( type[1] == 'r' )
            {
                CNAME(LexiRow, char)(xI, ind, *iflag, *m, *n, iord[0]);
            }
            else
            {
                CNAME(LexiCol, char)(xI, ind, *iflag, *m, *n, iord[0]);
            }
            break;
        case 'g' :
        default :
            GlobalSortchar(xI, ind, *iflag, *m, *n, iord[0]);
            break;
    }
    return(0);
}

int C2F(gsortuchar)(unsigned char *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord)
{

    switch ( type[0])
    {
        case 'r' :
            ColSortuchar(xI, ind, *iflag, *m, *n, iord[0]);
            break;
        case 'c' :
            RowSortuchar(xI, ind, *iflag, *m, *n, iord[0]);
            break;
        case 'l' :
            if ( type[1] == 'r' )
            {
                CNAME(LexiRow, uchar)(xI, ind, *iflag, *m, *n, iord[0]);
            }
            else
            {
                CNAME(LexiCol, uchar)(xI, ind, *iflag, *m, *n, iord[0]);
            }
            break;
        case 'g' :
        default :
            GlobalSortuchar(xI, ind, *iflag, *m, *n, iord[0]);
            break;
    }
    return(0);
}

/*--------------------------------------------------------------------------*/
/******************************************************
 * General sort routine for Scilab strings
 * iflag == if 1 ind is to be computed if 0 ind is ignored
 * m,n : matrix size
 * type : the operation ( see the interface )
 * iord : 'i' or 'd' : increasind or decreasing sort
 ******************************************************/
void C2F(gsorts)(char **data, int *ind, int *iflag, int *m, int *n, char *type, char *iord)
{

    switch ( type[0])
    {
        case 'r' :
            ColSortstring(data, ind, *iflag, *m, *n, iord[0]);
            break;
        case 'c' :
            RowSortstring(data, ind, *iflag, *m, *n, iord[0]);
            break;
        case 'l' :
            if ( type[1] == 'r' )
            {
                LexiRowstring(data, ind, *iflag, *m, *n, iord[0]);
            }
            else
            {
                LexiColstring(data, ind, *iflag, *m, *n, iord[0]);
            }
            break;
        case 'g' :
        default :
            GlobalSortstring(data, ind, *iflag, *m, *n, iord[0]);
            break;
    }
}
/*--------------------------------------------------------------------------*/
