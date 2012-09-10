/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#include "MALLOC.h"
#include "getRange.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
// =============================================================================
#define SIZE_ARRAY_RANGE 4
// =============================================================================
static int getSizeRange(const int *iRange, int nbRows, int nbCols);
static int getSizeRows(const int *iRange, int nbRows);
static int getSizeCols(const int *iRange, int nbCols);
// =============================================================================
void getSubIndices(const int *iRange, int * R1, int * R2, int * C1, int * C2 )
{
    *R1 = iRange[0];
    *C1 = iRange[1];
    *R2 = iRange[2];
    *C2 = iRange[3];

    return;
}
// =============================================================================
int isValidRange(const int *iRange, int sizeArray)
{
    int i = 0;
    int R1 = 0, C1 = 0, R2 = 0, C2 = 0;

    if (iRange == NULL)
    {
        return 0;
    }

    getSubIndices(iRange, &R1, &R2, &C1, &C2 );

    if (sizeArray != SIZE_ARRAY_RANGE)
    {
        return 0;
    }

    if (R1 <= 0)
    {
        return 0;
    }
    if (R2 <= 0)
    {
        return 0;
    }
    if (C1 <= 0)
    {
        return 0;
    }
    if (C2 <= 0)
    {
        return 0;
    }
    if (R1 > R2)
    {
        return 0;
    }
    if (C1 > C2)
    {
        return 0;
    }

    return 1;
}
// =============================================================================
char **getRangeAsString(const char **pStrsValues,
                        int nbRows, int nbCols,
                        const int *iRange,
                        int *returnedNbRows, int *returnedNbCols)
{
    if (isValidRange(iRange, SIZE_ARRAY_RANGE))
    {
        char **newStrArray = NULL;
        int sizeRange = getSizeRange(iRange, nbRows, nbCols);
        int R1, C1, R2, C2;

        getSubIndices(iRange, &R1, &R2, &C1, &C2 );

        *returnedNbRows = getSizeRows(iRange, nbRows);
        *returnedNbCols = getSizeCols(iRange, nbCols);

        newStrArray = (char**)MALLOC(sizeof(char*) * sizeRange);
        if (newStrArray == NULL)
        {
            *returnedNbCols = 0;
            *returnedNbRows = 0;
        }
        else
        {
            int i = 0;
            int j = 0;
            int k = 0;

            if ( C2 > nbCols )
            {
                C2 = nbCols;
            }
            if ( R2 > nbRows )
            {
                R2 = nbRows;
            }

            for ( j = C1 - 1 ; j < C2 ; j++)
            {
                for (i = R1 - 1 ; i < R2 ; i++ )
                {
                    newStrArray[k] = strdup(pStrsValues[i + nbRows * j]);
                    k++;
                }
            }
        }
        return newStrArray;
    }
    return NULL;
}
// =============================================================================
csv_complexArray *getRangeAsCsvComplexArray(const csv_complexArray *pComplex,
        int nbRows, int nbCols,
        const int *iRange,
        int *returnedNbRows, int *returnedNbCols)
{
    if (isValidRange(iRange, SIZE_ARRAY_RANGE))
    {
        csv_complexArray *newComplexArray = NULL;
        int sizeRange = getSizeRange(iRange, nbRows, nbCols);
        int R1 = 0, C1 = 0, R2 = 0, C2 = 0;

        getSubIndices(iRange, &R1, &R2, &C1, &C2 );

        *returnedNbRows = getSizeRows(iRange, nbRows);
        *returnedNbCols = getSizeCols(iRange, nbCols);

        newComplexArray = createCsvComplexArrayEmpty(sizeRange);
        if (newComplexArray == NULL)
        {
            *returnedNbCols = 0;
            *returnedNbRows = 0;
        }
        else
        {
            int i = 0;
            int j = 0;
            int k = 0;

            if ( C2 > nbCols )
            {
                C2 = nbCols;
            }
            if ( R2 > nbRows )
            {
                R2 = nbRows;
            }

            newComplexArray->isComplex = pComplex->isComplex;

            for ( j = C1 - 1 ; j < C2 ; j++)
            {
                for (i = R1 - 1 ; i < R2 ; i++)
                {
                    newComplexArray->realPart[k] = pComplex->realPart[i + (nbRows * j)];
                    if (pComplex->isComplex)
                    {
                        newComplexArray->imagPart[k] = pComplex->imagPart[i + (nbRows * j)];
                    }
                    k++;
                }
            }
        }

        return newComplexArray;
    }
    return NULL;
}
// =============================================================================
static int getSizeRange(const int *iRange, int nbRows, int nbCols)
{
    int sizeRange;
    if (iRange)
    {
        sizeRange = getSizeRows(iRange, nbRows) * getSizeCols(iRange, nbCols);
    }
    else
    {
        sizeRange = 0;
    }
    return sizeRange;
}
// =============================================================================
static int getSizeRows(const int *iRange, int nbRows)
{
    int sizeRows = 0;
    int R1, R2, C1, C2;

    getSubIndices(iRange, &R1, &R2, &C1, &C2 );

    if (iRange == NULL)
    {
        return 0;
    }
    if ( R2 > nbRows )
    {
        R2 = nbRows;
    }
    sizeRows = R2 - R1 + 1;
    return sizeRows;
}
// =============================================================================
static int getSizeCols(const int *iRange, int nbCols)
{
    int sizeCols = 0;
    int R1, R2, C1, C2;

    getSubIndices(iRange, &R1, &R2, &C1, &C2 );

    if (iRange == NULL)
    {
        return 0;
    }
    if ( C2 > nbCols )
    {
        C2 = nbCols;
    }
    sizeCols = C2 - C1 + 1;

    return sizeCols;
}
// =============================================================================
