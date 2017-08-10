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
#include "qsort-int.h"

/*--------------------------------------------------------------------------*/
static int compareCint(char *i, char *j, char *indi, char *indj, int iflag)
{
    if ( *((int *)i) > *((int *)j))
    {
        return (1);
    }
    if ( *((int *)i) < * ((int *)j))
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
static int compareDint(char *i, char *j, char *indi, char *indj, int iflag)
{
    if ( *((int *)i) < * ((int *)j))
    {
        return (1);
    }
    if ( *((int *)i) > *((int *)j))
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
static int compareCuint(char *i, char *j, char *indi, char *indj, int iflag)
{
    if ( *((unsigned int *)i) > *((unsigned int *)j))
    {
        return (1);
    }
    if ((unsigned int) * ((unsigned int *)i) < (unsigned int) * ((unsigned int *)j))
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
static int compareDuint(char *i, char *j, char *indi, char *indj, int iflag)
{
    if ((unsigned int) * ((unsigned int *)i) < (unsigned int) * ((unsigned int *)j))
    {
        return (1);
    }
    if ( (unsigned int) * ((unsigned int *)i) > (unsigned int) * ((unsigned int *)j))
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
void ColSortint(int *a, int *ind, int flag, int n, int p, char dir)
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
                 sizeof(int), sizeof(int),
                 (dir == 'i' ) ? compareCint : compareDint,
                 swapcodeint, swapcodeind);
    }
}
/*--------------------------------------------------------------------------*/
void ColSortuint(unsigned int *a, int *ind, int flag, int n, int p, char dir)
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
                 sizeof(int), sizeof(int),
                 (dir == 'i' ) ? compareCuint : compareDuint,
                 swapcodeint, swapcodeind);
    }
}
/*--------------------------------------------------------------------------*/

/******************************************************
 * Row sort of a matrix
 ******************************************************/
void RowSortint(int *a, int *ind, int flag, int n, int p, char dir)
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
                 n * sizeof(int), n * sizeof(int),
                 (dir == 'i' ) ? compareCint : compareDint,
                 swapcodeint, swapcodeind);
    }
}
/*--------------------------------------------------------------------------*/
void RowSortuint(unsigned int *a, int *ind, int flag, int n, int p, char dir)
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
                 n * sizeof(int), n * sizeof(int),
                 (dir == 'i' ) ? compareCuint : compareDuint,
                 swapcodeint, swapcodeind);
    }
}
/*--------------------------------------------------------------------------*/

/******************************************************
 * Global sort of a Matrix
 ******************************************************/
void GlobalSortint(int *a, int *ind, int flag, int n, int p, char dir)
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
             sizeof(int), sizeof(int),
             (dir == 'i' ) ? compareCint : compareDint,
             swapcodeint, swapcodeind);
}

/*--------------------------------------------------------------------------*/
void GlobalSortuint(unsigned int *a, int *ind, int flag, int n, int p, char dir)
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
             sizeof(int), sizeof(int),
             (dir == 'i' ) ? compareCuint : compareDuint,
             swapcodeint, swapcodeind);
}

/*--------------------------------------------------------------------------*/

/*******************************************************
 *  lexicographic order with Rows ind is of size n
 *  ind gives the permutation of the rows which is applied
 *  to sort them
 ******************************************************/
static int lexicolsint = 1;
static int lexirowsint = 1;
/*--------------------------------------------------------------------------*/
static void setLexiSizeint(int n, int p)
{
    lexicolsint = p;
    lexirowsint = n;
}

static  int LexiRowcompareCint(int *i, int *j)
{
    int jc;
    for ( jc = 0 ; jc < lexicolsint ; jc++)
    {
        if (*i > *j)
        {
            return (1);
        }
        if (*i < *j)
        {
            return (-1);
        }
        i += lexirowsint;
        j += lexirowsint;
    }
    return (0);
}

static  int LexiRowcompareCuint(unsigned int *i, unsigned int *j)
{
    int jc;
    for ( jc = 0 ; jc < lexicolsint ; jc++)
    {
        if (*i > *j)
        {
            return (1);
        }
        if (*i < *j)
        {
            return (-1);
        }
        i += lexirowsint;
        j += lexirowsint;
    }
    return (0);
}

static  int LexiRowcompareDint(int *i, int*j)
{
    int jc;
    for ( jc = 0 ; jc < lexicolsint ; jc++)
    {
        if (*i < *j)
        {
            return (1);
        }
        if (*i > *j)
        {
            return (-1);
        }
        i += lexirowsint;
        j += lexirowsint;
    }
    return (0);
}

static  int LexiRowcompareDuint(unsigned int *i, unsigned int*j)
{
    int jc;
    for ( jc = 0 ; jc < lexicolsint ; jc++)
    {
        if (*i < *j)
        {
            return (1);
        }
        if (*i > *j)
        {
            return (-1);
        }
        i += lexirowsint;
        j += lexirowsint;
    }
    return (0);
}

/*--------------------------------------------------------------------------*/
static int LexiRowswapcodeint(char *parmi, char * parmj, int n)
{
    int i = n, j;
    register int *pi = (int *) (parmi);
    register int *pj = (int *) (parmj);
    /* if ( n!= 1) printf(" swapcode avec n != 1\n"); */
    do
    {
        for ( j = 0 ; j < lexicolsint ; j++)
        {
            register int t = *(pi + lexirowsint * j);
            *(pi + lexirowsint * j) = *(pj + lexirowsint * j);
            *(pj + lexirowsint * j) = t;
        }
        pi++;
        pj++;
    }
    while (--i > 0);
    return (0);
}
/*--------------------------------------------------------------------------*/
void LexiRowint(int *a, int *ind, int flag, int n, int p, char dir)
{
    int i;
    setLexiSizeint(n, p);
    if ( flag == 1)
    {
        for ( i = 0 ; i < n ; i++)
        {
            ind[i] = i + 1;
        }
    }
    sciqsort((char *) (a), (char *) (ind), flag, n,
             sizeof(int), sizeof(int),
             (dir == 'i' ) ? LexiRowcompareCint : LexiRowcompareDint,
             LexiRowswapcodeint, swapcodeind);
}

void LexiRowuint(unsigned int *a, int *ind, int flag, int n, int p, char dir)
{
    int i;
    setLexiSizeint(n, p);
    if ( flag == 1)
    {
        for ( i = 0 ; i < n ; i++)
        {
            ind[i] = i + 1;
        }
    }
    sciqsort((char *) (a), (char *) (ind), flag, n,
             sizeof(int), sizeof(int),
             (dir == 'i' ) ? LexiRowcompareCuint : LexiRowcompareDuint,
             LexiRowswapcodeint, swapcodeind);
}

/*--------------------------------------------------------------------------*/
/******************************************************
 *  lexicographic order with Cols ind is of size p
 *  ind gives the permutation of the column which is applied
 *  to sort them
 ******************************************************/
static  int LexiColcompareCint(int *i, int *j)
{
    int ic;
    for ( ic = 0 ; ic < lexirowsint ; ic++)
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

static  int LexiColcompareCuint(unsigned int *i, unsigned int *j)
{
    int ic;
    for ( ic = 0 ; ic < lexirowsint ; ic++)
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
static  int LexiColcompareDint(int *i, int *j)
{
    int ic;
    for ( ic = 0 ; ic < lexirowsint ; ic++)
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

static  int LexiColcompareDuint(unsigned int *i, unsigned int *j)
{
    int ic;
    for ( ic = 0 ; ic < lexirowsint ; ic++)
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
static int LexiColswapcodeint(char *parmi, char* parmj, int n)
{
    int i = n, ir;
    register int *pi = (int *) (parmi);
    register int *pj = (int *) (parmj);
    /* if ( n!= 1) printf(" swapcode avec n != 1\n"); */
    do
    {
        for ( ir = 0 ; ir < lexirowsint ; ir++)
        {
            register int t = *(pi + ir);
            *(pi + ir) = *(pj + ir);
            *(pj + ir) = t;
        }
        pi += lexirowsint ;
        pj += lexirowsint ;
    }
    while (--i > 0);
    return (0);
}
/*--------------------------------------------------------------------------*/
void LexiColint(int *a, int *ind, int flag, int n, int p, char dir)
{
    int i;
    setLexiSizeint(n, p);
    if ( flag == 1)
    {
        for ( i = 0 ; i < p ; i++)
        {
            ind[i] = i + 1;
        }
    }
    sciqsort((char *) (a), (char *) (ind), flag, p,
             n * sizeof(int), sizeof(int),
             (dir == 'i' ) ? LexiColcompareCint : LexiColcompareDint,
             LexiColswapcodeint,
             swapcodeind);
}

void LexiColuint(unsigned int *a, int *ind, int flag, int n, int p, char dir)
{
    int i;
    setLexiSizeint(n, p);
    if ( flag == 1)
    {
        for ( i = 0 ; i < p ; i++)
        {
            ind[i] = i + 1;
        }
    }
    sciqsort((char *) (a), (char *) (ind), flag, p,
             n * sizeof(int), sizeof(int),
             (dir == 'i' ) ? LexiColcompareCuint : LexiColcompareDuint,
             LexiColswapcodeint,
             swapcodeind);
}

/*--------------------------------------------------------------------------*/

