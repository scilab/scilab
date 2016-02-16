/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ???? - ENPC - Jean-Philippe CHANCELIER
 * Copyright (C) 2006 - INRIA - Serge STEER
 * Copyright (C) 2006 - INRIA - Allan CORNET
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
 * Modified 2006 S.Steer A.Cornet (changing generic code to sepcialized code
 * by hand macro expansion).
 * Modified 2009 by S.Steer (to make in stable when index is wanted)
 */
#include <string.h>
#include "qsort.h"
#include "qsort-string.h"


static int swapcodestring( char ** parmi, char ** parmj, int n, int incr)
{
    int i = n;
    register char **pi = (char **) (parmi);
    register char **pj = (char **) (parmj);
    register int inc1 = incr / sizeof(char *);
    do
    {
        register char *t = *pi;
        *pi = *pj;
        *pj = t;
        pi += inc1;
        pj += inc1;
    }
    while (--i > 0);
    return (0);
}
/*--------------------------------------------------------------------------*/
static int compareCstring(char *i, char *j, char *indi, char *indj, int iflag)
{
    int r = strcmp(*((char * *) i), *((char **) j));
    if (r == 0 && iflag)
    {
        if ( *((int *)indi) > *((int *)indj))
        {
            r = 1;
        }
        if ( *((int *)indi) < * ((int *)indj))
        {
            r = -1;
        }
    }
    return (r);
}
/*--------------------------------------------------------------------------*/
static int compareDstring(char *i, char *j, char *indi, char *indj, int iflag)
{
    int r = -strcmp(*((char * *) i), *((char **) j));
    if (r == 0 && iflag)
    {
        if ( *((int *)indi) > *((int *)indj))
        {
            r = 1;
        }
        if ( *((int *)indi) < * ((int *)indj))
        {
            r = -1;
        }
    }
    return (r);
}
/*--------------------------------------------------------------------------*/
/******************************************************
 * Column sort of a matrix
 ******************************************************/
void ColSortstring(char * *a, int *ind, int flag, int n, int p, char dir)
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
                 sizeof(char *), sizeof(int),
                 (dir == 'i' ) ? compareCstring : compareDstring,
                 swapcodestring, swapcodeind);
    }
}
/*--------------------------------------------------------------------------*/
/******************************************************
 * Row sort of a matrix
 ******************************************************/
void RowSortstring(char * *a, int *ind, int flag, int n, int p, char dir)
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
                 n * sizeof(char *), n * sizeof(int),
                 (dir == 'i' ) ? compareCstring : compareDstring,
                 swapcodestring, swapcodeind);
    }
}
/*--------------------------------------------------------------------------*/
/******************************************************
 * Global sort of a Matrix
 ******************************************************/
void GlobalSortstring(char * *a, int *ind, int flag, int n, int p, char dir)
{
    int i;
    if ( flag == 1)
    {
        for ( i = 0 ; i < n * p ; i++)
        {
            ind[i] = i + 1;
        }
    }
    sciqsort((char *) (a), (char *) (ind), flag, n * p,
             sizeof(char *), sizeof(int),
             (dir == 'i' ) ? compareCstring : compareDstring,
             swapcodestring, swapcodeind);
}
/*--------------------------------------------------------------------------*/
/*******************************************************
 *  lexicographic order with Rows ind is of size n
 *  ind gives the permutation of the rows which is applied
 *  to sort them
 ******************************************************/
static int lexicolsstring = 1;
static int lexirowsstring = 1;
/*--------------------------------------------------------------------------*/
static void setLexiSizestring(int n, int p)
{
    lexicolsstring = p;
    lexirowsstring = n;
}
/*--------------------------------------------------------------------------*/
static  int LexiRowcompareCstring(char * *i, char * *j)
{
    int jc;
    for ( jc = 0 ; jc < lexicolsstring ; jc++)
    {
        int k = strcmp(*i, *j);
        if ( k != 0)
        {
            return (k);
        }
        i += lexirowsstring;
        j += lexirowsstring;
    }
    return (0);
}
/*--------------------------------------------------------------------------*/
static  int LexiRowcompareDstring(char * *i, char * *j)
{
    int jc;
    for ( jc = 0 ; jc < lexicolsstring ; jc++)
    {
        int k = strcmp(*i, *j);
        if ( k != 0)
        {
            return (-k);
        }
        i += lexirowsstring;
        j += lexirowsstring;
    }
    return (0);
}
/*--------------------------------------------------------------------------*/
static int LexiRowswapcodestring(char *parmi, char * parmj, int n)
{
    int i = n, j;
    register char * *pi = (char * *) (parmi);
    register char * *pj = (char * *) (parmj);
    /* if ( n!= 1) printf(" swapcode avec n != 1\n"); */
    do
    {
        for ( j = 0 ; j < lexicolsstring ; j++)
        {
            register char * t = *(pi + lexirowsstring * j);
            *(pi + lexirowsstring * j) = *(pj + lexirowsstring * j);
            *(pj + lexirowsstring * j) = t;
        }
        pi++;
        pj++;
    }
    while (--i > 0);
    return (0);
}
/*--------------------------------------------------------------------------*/
void LexiRowstring(char * *a, int *ind, int flag, int n, int p, char dir)
{
    int i;
    setLexiSizestring(n, p);
    if ( flag == 1)
    {
        for ( i = 0 ; i < n ; i++)
        {
            ind[i] = i + 1;
        }
    }
    sciqsort((char *) (a), (char *) (ind), flag, n,
             sizeof(char *), sizeof(int),
             (dir == 'i' ) ? LexiRowcompareCstring : LexiRowcompareDstring,
             LexiRowswapcodestring, swapcodeind);
}
/*--------------------------------------------------------------------------*/
/******************************************************
 *  lexicographic order with Cols ind is of size p
 *  ind gives the permutation of the column which is applied
 *  to sort them
 ******************************************************/
static  int LexiColcompareCstring(char * *i, char * *j)
{
    int ic;
    for ( ic = 0 ; ic < lexirowsstring ; ic++)
    {
        int k = strcmp(*i, *j);
        if ( k != 0)
        {
            return (k);
        }
        i++;
        j++;
    }
    return (0);
}
/*--------------------------------------------------------------------------*/
static  int LexiColcompareDstring(char * *i, char * *j)
{
    int ic;
    for ( ic = 0 ; ic < lexirowsstring ; ic++)
    {
        int k = strcmp(*i, *j);
        if ( k != 0)
        {
            return (-k);
        }
        i++;
        j++;
    }
    return (0);
}
/*--------------------------------------------------------------------------*/
static int LexiColswapcodestring(char *parmi, char* parmj, int n)
{
    int i = n, ir;
    register char * *pi = (char * *) (parmi);
    register char * *pj = (char * *) (parmj);
    /* if ( n!= 1) printf(" swapcode avec n != 1\n"); */
    do
    {
        for ( ir = 0 ; ir < lexirowsstring ; ir++)
        {
            register char * t = *(pi + ir);
            *(pi + ir) = *(pj + ir);
            *(pj + ir) = t;
        }
        pi += lexirowsstring ;
        pj += lexirowsstring ;
    }
    while (--i > 0);
    return (0);
}
/*--------------------------------------------------------------------------*/
void LexiColstring(char * *a, int *ind, int flag, int n, int p, char dir)
{
    int i;
    setLexiSizestring(n, p);
    if ( flag == 1)
    {
        for ( i = 0 ; i < p ; i++)
        {
            ind[i] = i + 1;
        }
    }
    sciqsort((char *) (a), (char *) (ind), flag, p,
             n * sizeof(char *), sizeof(int),
             (dir == 'i' ) ? LexiColcompareCstring : LexiColcompareDstring,
             LexiColswapcodestring, swapcodeind);
}
/*--------------------------------------------------------------------------*/
