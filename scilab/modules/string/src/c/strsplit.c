/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*----------------------------------------------------------------------------*/
#include <string.h>
#include "strsplit.h"
#include "sci_malloc.h"
#include "freeArrayOfString.h"
/*----------------------------------------------------------------------------*/
char** strsplit(const char* stringToSplit, double *indices, int sizeIndices, strsplit_error *ierr)
{
    char** splitted = NULL;
    *ierr = STRSPLIT_NO_ERROR;

    if (stringToSplit)
    {
        int lengthToCopy = 0;
        int lenString = (int)strlen(stringToSplit);
        int i = 0, j = 0;
        char* strDest = NULL;
        const char* strSrc = NULL;

        for (i = 0; i < sizeIndices; i++)
        {
            /* Check 2nd input matrix position */
            if ( ((int)indices[i] <= 0) || ((int)indices[i] >= lenString) )
            {
                *ierr = STRSPLIT_INCORRECT_VALUE_ERROR;
                return NULL;
            }

            /* check 2nd input order */
            if (sizeIndices > 1)
            {
                if ( i < (sizeIndices - 1) )
                {
                    if ((int)indices[i] > (int)indices[i + 1])
                    {
                        *ierr = STRSPLIT_INCORRECT_ORDER_ERROR;
                        return NULL;
                    }
                }
            }
        }

        splitted = (char**)MALLOC(sizeof(char*) * (sizeIndices + 1));
        if (splitted == NULL)
        {
            *ierr = STRSPLIT_MEMORY_ALLOCATION_ERROR;
            return NULL;
        }

        for (i = 0; i < sizeIndices; i++)
        {

            if (i == 0)
            {
                lengthToCopy = (int)indices[i];
            }
            else
            {
                lengthToCopy = (int)indices[i] - (int)indices[i - 1];
            }

            splitted[i] = (char*)MALLOC(sizeof(char) * (lengthToCopy + 1));
            strDest = splitted[i];

            if (splitted[i] == NULL)
            {
                freeArrayOfString(splitted, sizeIndices);
                *ierr = STRSPLIT_MEMORY_ALLOCATION_ERROR;
                return NULL;
            }
            
            strSrc = &stringToSplit[j];
            memcpy(strDest, strSrc, lengthToCopy * sizeof(char));
            strDest[lengthToCopy] = 0;

            j = (int)indices[i];
        }

        lengthToCopy = lenString - (int)indices[sizeIndices - 1];
        splitted[sizeIndices] = (char*)MALLOC(sizeof(char) * (lengthToCopy + 1));
        strDest = splitted[sizeIndices];

        if (splitted[sizeIndices] == NULL)
        {
            freeArrayOfString(splitted, sizeIndices + 1);
            *ierr = STRSPLIT_MEMORY_ALLOCATION_ERROR;
            return NULL;
        }

        strSrc = &stringToSplit[j];
        memcpy(strDest, strSrc, lengthToCopy * sizeof(char));
        strDest[lengthToCopy] = 0;
    }
    return splitted;
}
/*----------------------------------------------------------------------------*/
