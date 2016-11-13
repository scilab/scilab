/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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

/*------------------------------------------------------------------------*/
/* file: BasicAlgos.c                                                     */
/* desc : Several basic algorithms which can be used in several places in */
/*        the code.                                                       */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "BasicAlgos.h"
#include "sci_malloc.h"
#include "core_math.h"
#include "freeArrayOfString.h"
#include "returnanan.h"

/*------------------------------------------------------------------------*/
double sciFindStPosMin(const double x[], int n)
{
    double min = -1.0;
    int i = 0;
    char hasNeg = 0;

    if (n <= 0)
    {
        return min;
    }

    for (i = 0; i < n; i++)
    {
        if (x[i] > 0)
        {
            min = x[i];
            break;
        }
        else if (!hasNeg && x[i] <= 0)
        {
            hasNeg = 1;
        }
    }

    if (i == n && !hasNeg)
    {
        // we have only NaN
        return C2F(returnanan)();
    }

    for (; i < n ; i++)
    {
        if (x[i] > 0.0 && x[i] < min)
        {
            min = x[i];
        }
    }

    /* if we have found at least one positive value in x, min strictly positive */
    return min;
}
/*------------------------------------------------------------------------*/
int checkMonotony(const double vector[], int nbElement)
{
    int i = 0;
    if ( vector[1] >= vector[0] )
    {
        /* might be increasing */
        for ( i = 1 ; i < nbElement - 1 ; i++ )
        {
            if ( vector[i + 1] < vector[i] )
            {
                /* not increasing */
                return 0 ;
            }
        }
        return 1 ;
    }
    else
    {
        /* might be decreasing */
        for ( i = 1 ; i < nbElement - 1 ; i++ )
        {
            if ( vector[i + 1] > vector[i] )
            {
                /* not decreasing */
                return 0 ;
            }
        }
        return -1 ;

    }
    return 0 ;

}
/*------------------------------------------------------------------------*/
BOOL containsOneFiniteElement(const double vector[], int nbElement)
{
    int i = 0;
    for (i = 0; i < nbElement; i++)
    {
        if (finite(vector[i]))
        {
            return TRUE;
        }
    }
    return FALSE;
}
/*------------------------------------------------------------------------*/
void doubleArrayCopy(double dest[], const double src[], int nbElement)
{
    memcpy( dest, src, nbElement * sizeof(double) ) ;
}
/*------------------------------------------------------------------------*/
void intArrayCopy(int dest[], const int src[], int nbElement)
{
    memcpy( dest, src, nbElement * sizeof(int) ) ;
}
/*------------------------------------------------------------------------*/
BOOL stringArrayCopy(char * dest[], char * src[], int nbElement)
{
    int i = 0;
    for ( i = 0 ; i < nbElement ; i++ )
    {
        int elemSize =  (int)strlen( src[i] ) + 1 ;
        FREE( dest[i] ) ;

        dest[i] = MALLOC( elemSize * sizeof(char) ) ;

        if ( dest[i] == NULL )
        {
            destroyStringArray( dest, nbElement ) ;
            return FALSE;
        }

        strcpy( dest[i], src[i] ) ;
    }
    return TRUE;
}
/*------------------------------------------------------------------------*/
void setDoubleArraySingleValue(double dest[], double value, int nbElement)
{
    int i = 0;
    for ( i = 0 ; i < nbElement ; i++ )
    {
        dest[i] = value ;
    }
}
/*------------------------------------------------------------------------*/
double* createNewArrayFromSource(int destSize, const double src[], int srcSize)
{
    int i = 0;
    int endCopy = Min( destSize, srcSize ) ;
    /* create new array */
    double * dest = MALLOC( destSize * sizeof(double) ) ;

    if ( dest == NULL )
    {
        return NULL ;
    }

    /* copy the element which needs to be copied */
    memcpy( dest, src, endCopy * sizeof( double ) ) ;

    for ( i = endCopy ; i < destSize ; i++ )
    {
        dest[i] = 0.0 ;
    }

    return dest ;

}
/*------------------------------------------------------------------------*/
void destroyStringArray(char * src[], int nbStrings)
{
    freeArrayOfString(src, nbStrings);
}
/*--------------------------------------------------------------------------*/
double* createDoubleArrayCopy(const double src[], int nbElement)
{
    double * res = MALLOC( nbElement * sizeof(double) ) ;

    if ( res == NULL )
    {
        return NULL ;
    }

    memcpy( res, src, nbElement * sizeof(double) ) ;

    return res ;
}
/*--------------------------------------------------------------------------*/
int * createIntArrayCopy(const int src[], int nbElement)
{
    int * res = MALLOC( nbElement * sizeof(int) ) ;

    if ( res == NULL )
    {
        return NULL ;
    }

    memcpy( res, src, nbElement * sizeof(int) ) ;

    return res ;
}
/*--------------------------------------------------------------------------*/
char ** createStringArrayCopy(char * src[], int nbElement)
{
    char ** res = createStringArray(nbElement);
    if ( res == NULL )
    {
        return NULL ;
    }

    if ( stringArrayCopy( res, src, nbElement ) )
    {
        return res ;
    }
    else
    {
        return NULL;
    }


}
/*--------------------------------------------------------------------------*/
char ** createStringArray(int nbElement)
{
    int i = 0;
    char ** res = MALLOC( nbElement * sizeof(char *) ) ;

    if ( res == NULL )
    {
        return NULL ;
    }

    for ( i = 0 ; i < nbElement; i++ )
    {
        res[i] = NULL ;
    }

    return res ;
}
/*--------------------------------------------------------------------------*/
