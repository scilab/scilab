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
#include "qsort-short.h"

static int swapcodeshort(char * parmi, char * parmj, int n, int incr)
{
    int i = n;
    register short *pi = (short *) (parmi);
    register short *pj = (short *) (parmj);
    register int inc1 = incr / sizeof(short);
    do
    {
        register short t = *pi;
        *pi = *pj;
        *pj = t;
        pi += inc1;
        pj += inc1;
    }
    while (--i > 0);
    return (0);
}
/*--------------------------------------------------------------------------*/
static int compareCshort(char *i, char *j, char *indi, char *indj, int iflag)
{
    if ( *((short *)i) > *((short *)j))
    {
        return (1);
    }
    if ( *((short *)i) < * ((short *)j))
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
static int compareDshort(char *i, char *j, char *indi, char *indj, int iflag)
{
    if ( *((short *)i) < * ((short *)j))
    {
        return (1);
    }
    if ( *((short *)i) > *((short *)j))
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
static int compareCushort(char *i, char *j, char *indi, char *indj, int iflag)
{
    if ( *((unsigned short *)i) > *((unsigned short *)j))
    {
        return (1);
    }
    if ( *((unsigned short *)i) < * ((unsigned short *)j))
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
static int compareDushort(char *i, char *j, char *indi, char *indj, int iflag)
{
    if ( *((unsigned short *)i) < * ((unsigned short *)j))
    {
        return (1);
    }
    if ( *((unsigned short *)i) > *((unsigned short *)j))
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
void ColSortshort(short *a, int *ind, int flag, int n, int p, char dir)
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
                 sizeof(short), sizeof(int),
                 (dir == 'i' ) ? compareCshort : compareDshort,
                 swapcodeshort, swapcodeind);
    }
}
/*--------------------------------------------------------------------------*/
/******************************************************
 * Row sort of a matrix
 ******************************************************/
void RowSortshort(short *a, int *ind, int flag, int n, int p, char dir)
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
                 n * sizeof(short), n * sizeof(int),
                 (dir == 'i' ) ? compareCshort : compareDshort,
                 swapcodeshort, swapcodeind);
    }
}
/*--------------------------------------------------------------------------*/
/******************************************************
 * Global sort of a Matrix
 ******************************************************/
void GlobalSortshort(short *a, int *ind, int flag, int n, int p, char dir)
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
             sizeof(short), sizeof(int),
             (dir == 'i' ) ? compareCshort : compareDshort,
             swapcodeshort, swapcodeind);
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/******************************************************
 * Column sort of a matrix
 ******************************************************/
void ColSortushort(unsigned short *a, int *ind, int flag, int n, int p, char dir)
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
                 sizeof(short), sizeof(int),
                 (dir == 'i' ) ? compareCushort : compareDushort,
                 swapcodeshort, swapcodeind);
    }
}
/*--------------------------------------------------------------------------*/
/******************************************************
 * Row sort of a matrix
 ******************************************************/
void RowSortushort(unsigned short *a, int *ind, int flag, int n, int p, char dir)
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
                 n * sizeof(short), n * sizeof(int),
                 (dir == 'i' ) ? compareCushort : compareDushort,
                 swapcodeshort, swapcodeind);
    }
}
/*--------------------------------------------------------------------------*/
/******************************************************
 * Global sort of a Matrix
 ******************************************************/
void GlobalSortushort(unsigned short *a, int *ind, int flag, int n, int p, char dir)
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
             sizeof(short), sizeof(int),
             (dir == 'i' ) ? compareCushort : compareDushort,
             swapcodeshort, swapcodeind);
}
/*--------------------------------------------------------------------------*/
/*******************************************************
 *  lexicographic order with Rows ind is of size n
 *  ind gives the permutation of the rows which is applied
 *  to sort them
 ******************************************************/
static int lexicolsshort = 1;
static int lexirowsshort = 1;
/*--------------------------------------------------------------------------*/
static void setLexiSizeshort(int n, int p)
{
    lexicolsshort = p;
    lexirowsshort = n;
}

static  int LexiRowcompareCshort(short *i, short *j)
{
    int jc;
    for ( jc = 0 ; jc < lexicolsshort ; jc++)
    {
        if (*i > *j)
        {
            return (1);
        }
        if (*i < *j)
        {
            return (-1);
        }
        i += lexirowsshort;
        j += lexirowsshort;
    }
    return (0);
}
static  int LexiRowcompareDshort(short *i, short*j)
{
    int jc;
    for ( jc = 0 ; jc < lexicolsshort ; jc++)
    {
        if (*i < *j)
        {
            return (1);
        }
        if (*i > *j)
        {
            return (-1);
        }
        i += lexirowsshort;
        j += lexirowsshort;
    }
    return (0);
}
static  int LexiRowcompareCushort(unsigned short *i, unsigned short *j)
{
    int jc;
    for ( jc = 0 ; jc < lexicolsshort ; jc++)
    {
        if (*i > *j)
        {
            return (1);
        }
        if (*i < *j)
        {
            return (-1);
        }
        i += lexirowsshort;
        j += lexirowsshort;
    }
    return (0);
}
static  int LexiRowcompareDushort(unsigned short *i, unsigned short*j)
{
    int jc;
    for ( jc = 0 ; jc < lexicolsshort ; jc++)
    {
        if (*i < *j)
        {
            return (1);
        }
        if (*i > *j)
        {
            return (-1);
        }
        i += lexirowsshort;
        j += lexirowsshort;
    }
    return (0);
}
/*--------------------------------------------------------------------------*/
static int LexiRowswapcodeshort(char *parmi, char * parmj, int n)
{
    int i = n, j;
    register short *pi = (short *) (parmi);
    register short *pj = (short *) (parmj);
    /* if ( n!= 1) printf(" swapcode avec n != 1\n"); */
    do
    {
        for ( j = 0 ; j < lexicolsshort ; j++)
        {
            register short t = *(pi + lexirowsshort * j);
            *(pi + lexirowsshort * j) = *(pj + lexirowsshort * j);
            *(pj + lexirowsshort * j) = t;
        }
        pi++;
        pj++;
    }
    while (--i > 0);
    return (0);
}
/*--------------------------------------------------------------------------*/
void LexiRowshort(short *a, int *ind, int flag, int n, int p, char dir)
{
    int i;
    setLexiSizeshort(n, p);
    if ( flag == 1)
    {
        for ( i = 0 ; i < n ; i++)
        {
            ind[i] = i + 1;
        }
    }
    sciqsort((char *) (a), (char *) (ind), flag, n,
             sizeof(short), sizeof(int),
             (dir == 'i' ) ? LexiRowcompareCshort : LexiRowcompareDshort,
             LexiRowswapcodeshort, swapcodeind);
}

/*--------------------------------------------------------------------------*/
void LexiRowushort(unsigned short *a, int *ind, int flag, int n, int p, char dir)
{
    int i;
    setLexiSizeshort(n, p);
    if ( flag == 1)
    {
        for ( i = 0 ; i < n ; i++)
        {
            ind[i] = i + 1;
        }
    }
    sciqsort((char *) (a), (char *) (ind), flag, n,
             sizeof(short), sizeof(int),
             (dir == 'i' ) ? LexiRowcompareCushort : LexiRowcompareDushort,
             LexiRowswapcodeshort, swapcodeind);
}
/*--------------------------------------------------------------------------*/
/******************************************************
 *  lexicographic order with Cols ind is of size p
 *  ind gives the permutation of the column which is applied
 *  to sort them
 ******************************************************/
static  int LexiColcompareCshort(short *i, short *j)
{
    int ic;
    for ( ic = 0 ; ic < lexirowsshort ; ic++)
    {
        if (*i > *j)
        {
            return (1);
        }
        if (*i < *j)
        {
            return (-1);
        }
        i++;
        j++;
    }
    return (0);
}
/*--------------------------------------------------------------------------*/
static  int LexiColcompareDshort(short *i, short *j)
{
    int ic;
    for ( ic = 0 ; ic < lexirowsshort ; ic++)
    {
        if (*i < *j)
        {
            return (1);
        }
        if (*i > *j)
        {
            return (-1);
        }
        i++;
        j++;
    }
    return (0);
}
/*--------------------------------------------------------------------------*/
static  int LexiColcompareCushort(unsigned short *i, unsigned short *j)
{
    int ic;
    for ( ic = 0 ; ic < lexirowsshort ; ic++)
    {
        if (*i > *j)
        {
            return (1);
        }
        if (*i < *j)
        {
            return (-1);
        }
        i++;
        j++;
    }
    return (0);
}
/*--------------------------------------------------------------------------*/
static  int LexiColcompareDushort(unsigned short *i, unsigned short *j)
{
    int ic;
    for ( ic = 0 ; ic < lexirowsshort ; ic++)
    {
        if (*i < *j)
        {
            return (1);
        }
        if (*i > *j)
        {
            return (-1);
        }
        i++;
        j++;
    }
    return (0);
}

/*--------------------------------------------------------------------------*/
static int LexiColswapcodeshort(char *parmi, char* parmj, int n)
{
    int i = n, ir;
    register short *pi = (short *) (parmi);
    register short *pj = (short *) (parmj);
    /* if ( n!= 1) printf(" swapcode avec n != 1\n"); */
    do
    {
        for ( ir = 0 ; ir < lexirowsshort ; ir++)
        {
            register short t = *(pi + ir);
            *(pi + ir) = *(pj + ir);
            *(pj + ir) = t;
        }
        pi += lexirowsshort ;
        pj += lexirowsshort ;
    }
    while (--i > 0);
    return (0);
}
/*--------------------------------------------------------------------------*/
void LexiColshort(short *a, int *ind, int flag, int n, int p, char dir)
{
    int i;
    setLexiSizeshort(n, p);
    if ( flag == 1)
    {
        for ( i = 0 ; i < p ; i++)
        {
            ind[i] = i + 1;
        }
    }
    sciqsort((char *) (a), (char *) (ind), flag, p,
             n * sizeof(short), sizeof(int),
             (dir == 'i' ) ? LexiColcompareCshort : LexiColcompareDshort,
             LexiColswapcodeshort,
             swapcodeind);
}
/*--------------------------------------------------------------------------*/
void LexiColushort(unsigned short *a, int *ind, int flag, int n, int p, char dir)
{
    int i;
    setLexiSizeshort(n, p);
    if ( flag == 1)
    {
        for ( i = 0 ; i < p ; i++)
        {
            ind[i] = i + 1;
        }
    }
    sciqsort((char *) (a), (char *) (ind), flag, p,
             n * sizeof(short), sizeof(int),
             (dir == 'i' ) ? LexiColcompareCushort : LexiColcompareDushort,
             LexiColswapcodeshort,
             swapcodeind);
}
/*--------------------------------------------------------------------------*/

