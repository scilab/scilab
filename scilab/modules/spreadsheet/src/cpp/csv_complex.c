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
#include "csv_complex.h"
#include "MALLOC.h"
// =============================================================================
csv_complexArray *createCsvComplexArrayEmpty(int nbSize)
{
    csv_complexArray *pCsvComplexArray = NULL;
    pCsvComplexArray = (csv_complexArray *) MALLOC(sizeof(csv_complexArray));
    if (pCsvComplexArray)
    {
        pCsvComplexArray->realPart = (double *)MALLOC(sizeof(double) * nbSize);
        if (pCsvComplexArray->realPart == NULL)
        {
            FREE(pCsvComplexArray);
            return NULL;
        }

        pCsvComplexArray->imagPart = (double *)MALLOC(sizeof(double) * nbSize);
        if (pCsvComplexArray->imagPart == NULL)
        {
            FREE(pCsvComplexArray->realPart);
            pCsvComplexArray->realPart = NULL;
            FREE(pCsvComplexArray);
            return NULL;
        }

        memset(pCsvComplexArray->realPart, 0, sizeof(double) * nbSize);
        memset(pCsvComplexArray->imagPart, 0, sizeof(double) * nbSize);
        pCsvComplexArray->isComplex = 1;
        pCsvComplexArray->nbElements = nbSize;
    }
    return pCsvComplexArray;
}
// =============================================================================
csv_complexArray *createCsvComplexArray(const double *realpart, const double *imagpart, int nbSize)
{
    csv_complexArray *pCsvComplexArray = NULL;

    if (realpart == NULL)
    {
        return pCsvComplexArray;
    }

    if (nbSize <= 0)
    {
        return pCsvComplexArray;
    }

    pCsvComplexArray = (csv_complexArray *) MALLOC(sizeof(csv_complexArray));
    if (pCsvComplexArray)
    {
        if (imagpart == NULL)
        {
            pCsvComplexArray->isComplex = 0;
        }
        else
        {
            int i = 0;
            pCsvComplexArray->isComplex = 0;
            for (i = 0; i < nbSize; i++)
            {
                if (imagpart[i] != 0)
                {
                    pCsvComplexArray->isComplex = 1;
                    break;
                }
            }
        }

        pCsvComplexArray->realPart = (double *)MALLOC(sizeof(double) * nbSize);
        if (pCsvComplexArray->realPart)
        {
            memcpy(pCsvComplexArray->realPart, realpart, nbSize);
            pCsvComplexArray->nbElements = nbSize;
        }
        if (pCsvComplexArray->isComplex)
        {
            pCsvComplexArray->imagPart = (double *)MALLOC(sizeof(double) * nbSize);
            if (pCsvComplexArray->imagPart)
            {
                memcpy(pCsvComplexArray->imagPart, imagpart, nbSize);
            }
        }
        else
        {
            pCsvComplexArray->imagPart = NULL;
        }
    }
    return pCsvComplexArray;
}
// =============================================================================
void freeCsvComplexArray(csv_complexArray *pCsvComplexArray)
{
    if (pCsvComplexArray)
    {
        if (pCsvComplexArray->realPart)
        {
            FREE(pCsvComplexArray->realPart);
            pCsvComplexArray->realPart = NULL;
        }

        if (pCsvComplexArray->imagPart)
        {
            FREE(pCsvComplexArray->imagPart);
            pCsvComplexArray->imagPart = NULL;
        }
        pCsvComplexArray->isComplex = 0;
        pCsvComplexArray->nbElements = 0;

        FREE(pCsvComplexArray);
        pCsvComplexArray = NULL;
    }
}
// =============================================================================
int cleanImagPartCsvComplexArray(csv_complexArray *pCsvComplexArray)
{
    if (pCsvComplexArray)
    {
        if (pCsvComplexArray->imagPart)
        {
            int i = 0;
            int isComplex = 0;
            for (i = 0; i < pCsvComplexArray->nbElements; i++)
            {
                if (pCsvComplexArray->imagPart[i] != 0)
                {
                    isComplex = 1;
                    break;
                }
            }

            if (isComplex)
            {
                pCsvComplexArray->isComplex = 1;
            }
            else
            {
                pCsvComplexArray->isComplex = 0;
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
