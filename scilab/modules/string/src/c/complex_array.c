/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include <string.h>
#include "complex_array.h"
#include "MALLOC.h"
// =============================================================================
complexArray *createComplexArrayEmpty(int nbSize)
{
    complexArray *pComplexArray = NULL;
    pComplexArray = (complexArray *) MALLOC(sizeof(complexArray));
    if (pComplexArray)
    {
        pComplexArray->realPart = (double *)MALLOC(sizeof(double) * nbSize);
        if (pComplexArray->realPart == NULL)
        {
            FREE(pComplexArray);
            return NULL;
        }

        pComplexArray->imagPart = (double *)MALLOC(sizeof(double) * nbSize);
        if (pComplexArray->imagPart == NULL)
        {
            FREE(pComplexArray->realPart);
            pComplexArray->realPart = NULL;
            FREE(pComplexArray);
            return NULL;
        }

        memset(pComplexArray->realPart, 0, sizeof(double) * nbSize);
        memset(pComplexArray->imagPart, 0, sizeof(double) * nbSize);
        pComplexArray->isComplex = 1;
        pComplexArray->nbElements = nbSize;
    }
    return pComplexArray;
}
// =============================================================================
complexArray *createComplexArray(const double *realpart, const double *imagpart, int nbSize)
{
    complexArray *pComplexArray = NULL;

    if (realpart == NULL)
    {
        return pComplexArray;
    }

    if (nbSize <= 0)
    {
        return pComplexArray;
    }

    pComplexArray = (complexArray *) MALLOC(sizeof(complexArray));
    if (pComplexArray)
    {
        if (imagpart == NULL)
        {
            pComplexArray->isComplex = 0;
        }
        else
        {
            int i = 0;
            pComplexArray->isComplex = 0;
            for (i = 0; i < nbSize; i++)
            {
                if (imagpart[i] != 0)
                {
                    pComplexArray->isComplex = 1;
                    break;
                }
            }
        }

        pComplexArray->realPart = (double *)MALLOC(sizeof(double) * nbSize);
        if (pComplexArray->realPart)
        {
            memcpy(pComplexArray->realPart, realpart, nbSize);
            pComplexArray->nbElements = nbSize;
        }
        if (pComplexArray->isComplex)
        {
            pComplexArray->imagPart = (double *)MALLOC(sizeof(double) * nbSize);
            if (pComplexArray->imagPart)
            {
                memcpy(pComplexArray->imagPart, imagpart, nbSize);
            }
        }
        else
        {
            pComplexArray->imagPart = NULL;
        }
    }
    return pComplexArray;
}
// =============================================================================
void freeComplexArray(complexArray *pComplexArray)
{
    if (pComplexArray)
    {
        if (pComplexArray->realPart)
        {
            FREE(pComplexArray->realPart);
            pComplexArray->realPart = NULL;
        }

        if (pComplexArray->imagPart)
        {
            FREE(pComplexArray->imagPart);
            pComplexArray->imagPart = NULL;
        }
        pComplexArray->isComplex = 0;
        pComplexArray->nbElements = 0;

        FREE(pComplexArray);
        pComplexArray = NULL;
    }
}
// =============================================================================
int cleanImagPartComplexArray(complexArray *pComplexArray)
{
    if (pComplexArray)
    {
        if (pComplexArray->imagPart)
        {
            int i = 0;
            int isComplex = 0;
            for (i = 0; i < pComplexArray->nbElements; i++)
            {
                if (pComplexArray->imagPart[i] != 0)
                {
                    isComplex = 1;
                    break;
                }
            }

            if (isComplex)
            {
                pComplexArray->isComplex = 1;
            }
            else
            {
                pComplexArray->isComplex = 0;
            }
        }
        else
        {
            return 1;
        }
    }
    return 0;
}
// =============================================================================
