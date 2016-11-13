/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ???? - ENPC - Jean-Philippe CHANCELIER
 * Copyright (C) 2006 - INRIA - Serge STEER
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

/*
 * Modified 2006 by S.Steer and A.Cornet INRIA  (changing generic code to sepcialized code
 * by hand macro expansion).
 * Modified 2009 by S.Steer  INRIA (to make in stable when index is wanted)
 */


#include "qsort.h"
#include "qsort-double.h"
#include "isanan.h"

static int swapcodedouble(char *  parmi, char * parmj, int  n, int inc)
{
    int i = n;
    register double *pi = (double *) (parmi);
    register double *pj = (double *) (parmj);
    register int inc1 = inc / sizeof(double);
    do
    {
        register double t = *pi;
        *pi = *pj;
        *pj = t;
        pi += inc1;
        pj += inc1;
    }
    while (--i > 0);
    return (0);
}
/*--------------------------------------------------------------------------*/
static int compareCdouble(char *i, char *j, char *indi, char *indj, int iflag)
{
    if ( *((double *)i) > *((double *)j) || C2F(isanan)((double *)i) == 1)
    {
        return (1);
    }
    if ( *((double *)i) < * ((double *)j) || C2F(isanan)((double *)j) == 1)
    {
        return (-1);
    }
    if (iflag)
    {
        if ( *((int *)indi) > *((int *)indj))
        {
            return (1);
        }
        if ( *((int *)indi) < * ((int *)indj))
        {
            return (-1);
        }
    }
    return (0);
}
/*--------------------------------------------------------------------------*/
static int compareDdouble(char *i, char *j, char *indi, char *indj, int iflag)
{
    if ( *((double *)i) < * ((double *)j) || C2F(isanan)((double *)j) == 1)
    {
        return (1);
    }
    if ( *((double *)i) > *((double *)j) || C2F(isanan)((double *)i) == 1)
    {
        return (-1);
    }
    if (iflag)
    {
        if ( *((int *)indi) > *((int *)indj))
        {
            return (1);
        }
        if ( *((int *)indi) < * ((int *)indj))
        {
            return (-1);
        }
    }
    return (0);
}
/*--------------------------------------------------------------------------*/
/******************************************************
 * Column sort of a matrix
 ******************************************************/
void ColSortdouble(double *a, int *ind, int flag, int n, int p, char dir)
{
    int i, j;
    if ( flag == 1)
    {
        for ( j = 0 ; j < p ; j++ )
        {
            for ( i = 0 ; i < n ; i++)
            {
                ind[i + n * j] = i + 1;
            }
        }
    }

    for ( j = 0 ; j < p ; j++ )
    {
        sciqsort((char *) (a + n * j), (char *) (ind + n * j), flag, n,
                 sizeof(double), sizeof(int),
                 (dir == 'i' ) ? compareCdouble : compareDdouble,
                 swapcodedouble, swapcodeind);
    }
}
/*--------------------------------------------------------------------------*/
/******************************************************
 * Row sort of a matrix
 ******************************************************/
void RowSortdouble(double *a, int *ind, int flag, int n, int p, char dir)
{
    int i, j;
    if ( flag == 1)
    {
        for ( i = 0 ; i < n ; i++)
        {
            for ( j = 0 ; j < p ; j++ )
            {
                ind[i + n * j] = j + 1;
            }
        }
    }
    for ( i = 0 ; i < n ; i++)
    {
        sciqsort((char *) (a + i), (char *) (ind + i), flag, p,
                 n * sizeof(double), n * sizeof(int),
                 (dir == 'i' ) ? compareCdouble : compareDdouble,
                 swapcodedouble, swapcodeind);
    }
}
/*--------------------------------------------------------------------------*/
/******************************************************
 * Global sort of a Matrix
 ******************************************************/
void GlobalSortdouble(double *a, int *ind, int flag, int n, int p, char dir)
{
    int i;
    if ( flag == 1) for ( i = 0 ; i < n * p ; i++)
        {
            ind[i] = i + 1;
        }
    sciqsort((char *) (a), (char *) (ind), flag, n * p,
             sizeof(double), sizeof(int),
             (dir == 'i' ) ? compareCdouble : compareDdouble,
             swapcodedouble, swapcodeind);
}
/*--------------------------------------------------------------------------*/
/*******************************************************
 *  lexicographic order with Rows ind is of size n
 *  ind gives the permutation of the rows which is applied
 *  to sort them
 ******************************************************/
static int lexicolsdouble = 1;
static int lexirowsdouble = 1;
/*--------------------------------------------------------------------------*/
static void setLexiSizedouble(int n, int p)
{
    lexicolsdouble = p;
    lexirowsdouble = n;
}
/*--------------------------------------------------------------------------*/
static  int LexiRowcompareCdouble(double *i, double *j)
{
    int jc;

    for ( jc = 0 ; jc < lexicolsdouble ; jc++)
    {
        if (*i > *j || (double *)C2F(isanan)((double *)i) == (double *) 1)
        {
            return (1);
        }
        if (*i < *j || (double *)C2F(isanan)((double *)j) == (double *) 1)
        {
            return (-1);
        }
        i += lexirowsdouble;
        j += lexirowsdouble;
    }
    return (0);
}
/*--------------------------------------------------------------------------*/
static  int LexiRowcompareDdouble(double *i, double*j)
{
    int jc;

    for ( jc = 0 ; jc < lexicolsdouble ; jc++)
    {
        if (*i < *j || (double *)C2F(isanan)((double *)j) == (double *) 1)
        {
            return (1);
        }
        if (*i > *j || (double *)C2F(isanan)((double *)i) == (double *) 1)
        {
            return (-1);
        }
        i += lexirowsdouble;
        j += lexirowsdouble;
    }
    return (0);
}
/*--------------------------------------------------------------------------*/
static int LexiRowswapcodedouble(char *parmi, char * parmj, int n)
{
    int i = n, j;
    register double *pi = (double *) (parmi);
    register double *pj = (double *) (parmj);

    do
    {
        for ( j = 0 ; j < lexicolsdouble ; j++)
        {
            register double t = *(pi + lexirowsdouble * j);
            *(pi + lexirowsdouble * j) = *(pj + lexirowsdouble * j);
            *(pj + lexirowsdouble * j) = t;
        }
        pi++;
        pj++;
    }
    while (--i > 0);
    return (0);
}
/*--------------------------------------------------------------------------*/
void LexiRowdouble(double *a, int *ind, int flag, int n, int p, char dir)
{
    int i;
    setLexiSizedouble(n, p);
    if ( flag == 1)
    {
        for ( i = 0 ; i < n ; i++)
        {
            ind[i] = i + 1;
        }
    }
    sciqsort((char *) (a), (char *) (ind), flag, n,
             sizeof(double), sizeof(int),
             (dir == 'i' ) ? LexiRowcompareCdouble : LexiRowcompareDdouble,
             LexiRowswapcodedouble, swapcodeind);
}
/*--------------------------------------------------------------------------*/
/******************************************************
 *  lexicographic order with Cols ind is of size p
 *  ind gives the permutation of the column which is applied
 *  to sort them
 ******************************************************/
static  int LexiColcompareCdouble(double *i, double *j)
{
    int ic;
    for ( ic = 0 ; ic < lexirowsdouble ; ic++)
    {
        if (*i > *j || (double *)C2F(isanan)((double *)i) == (double *) 1)
        {
            return (1);
        }
        if (*i < *j || (double *)C2F(isanan)((double *)j) == (double *) 1)
        {
            return (-1);
        }
        i++;
        j++;
    }
    return (0);
}
/*--------------------------------------------------------------------------*/
static  int LexiColcompareDdouble(double *i, double *j)
{
    int ic;
    for ( ic = 0 ; ic < lexirowsdouble ; ic++)
    {
        if (*i < *j || (double *)C2F(isanan)((double *)j) == (double *) 1)
        {
            return (1);
        }
        if (*i > *j || (double *)C2F(isanan)((double *)i) ==  (double *) 1)
        {
            return (-1);
        }
        i++;
        j++;
    }
    return (0);
}
/*--------------------------------------------------------------------------*/
static int LexiColswapcodedouble(char *parmi, char* parmj, int n)
{
    int i = n, ir;
    register double *pi = (double *) (parmi);
    register double *pj = (double *) (parmj);
    do
    {
        for ( ir = 0 ; ir < lexirowsdouble ; ir++)
        {
            register double t = *(pi + ir);
            *(pi + ir) = *(pj + ir);
            *(pj + ir) = t;
        }
        pi += lexirowsdouble ;
        pj += lexirowsdouble ;
    }
    while (--i > 0);
    return (0);
}
/*--------------------------------------------------------------------------*/
void LexiColdouble(double *a, int *ind, int flag, int n, int p, char dir)
{
    int i;
    setLexiSizedouble(n, p);
    if ( flag == 1)
    {
        for ( i = 0 ; i < p ; i++)
        {
            ind[i] = i + 1;
        }
    }
    sciqsort((char *) (a), (char *) (ind), flag, p,
             n * sizeof(double), sizeof(int),
             (dir == 'i' ) ? LexiColcompareCdouble : LexiColcompareDdouble,
             LexiColswapcodedouble,
             swapcodeind);
}
/*--------------------------------------------------------------------------*/

