/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "machine.h" /* bsiz */
#include "convertbase.h"
#include "sci_malloc.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
#define char_a 'a'
#define char_A 'A'
#define char_z 'z'
#define char_Z 'Z'
#define char_zero '0'
#define char_nine '9'
/*--------------------------------------------------------------------------*/
static char *convertDec2Base(double dValue, int numberbase,
                             unsigned int nbDigits, error_convertbase *err);
/*--------------------------------------------------------------------------*/
double convertBase2Dec(const char *pStr, int numberbase, error_convertbase *err)
{
    double result = 0.;
    *err = ERROR_CONVERTBASE_NOK;

    if (pStr)
    {
        size_t i = 0;
        size_t len = strlen(pStr);
        for (i = 0; i < len; i++)
        {
            if ((pStr[i] >= char_zero) && (pStr[i] <= char_nine))
            {
                result = (numberbase * result) + (int)pStr[i] - char_zero;
            }
            else if ((pStr[i] >= char_A) && (pStr[i] <= char_Z))
            {
                int vTmp = (int)pStr[i] - char_A + 10;
                if (vTmp > numberbase)
                {
                    *err = ERROR_CONVERTBASE_INVALID_REPRESENTATION;
                    return 0;
                }
                else
                {
                    result = (numberbase * result) + vTmp;
                }
            }
            else if ((pStr[i] >= char_a) && (pStr[i] <= char_z))
            {
                int vTmp = (int)pStr[i] - char_a + 10;
                if ( vTmp > numberbase)
                {
                    *err = ERROR_CONVERTBASE_INVALID_REPRESENTATION;
                    return 0;
                }
                else
                {
                    result = (numberbase * result) + vTmp;
                }
            }
            else
            {
                return 0;
            }
        }
        *err = ERROR_CONVERTBASE_OK;
    }
    return result;
}
/*--------------------------------------------------------------------------*/
static char *convertDec2Base(double dValue, int numberbase,
                             unsigned int nbDigits, error_convertbase *err)
{
    char symbols[37] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    long long int iDec = (long long int) dValue;
    char *convertedValue = NULL;

    *err = ERROR_CONVERTBASE_NOK;
    if (iDec == 0)
    {
        convertedValue = (char*)MALLOC(sizeof(char) * 2);
        if (convertedValue)
        {
            strcpy(convertedValue, "0");
            *err = ERROR_CONVERTBASE_OK;
        }
        else
        {
            *err = ERROR_CONVERTBASE_ALLOCATION;
            return NULL;
        }
    }
    else
    {
        int count = 0;
        char chResult[bsiz] = "";
        char *pChResult = chResult;
        while (iDec > 0 && count++ < bsiz)
        {
            *pChResult = symbols[iDec % numberbase];
            pChResult++;
            iDec = iDec / numberbase;
        }

        convertedValue = (char*)MALLOC(sizeof(char) * (strlen(chResult) + 1));
        if (convertedValue)
        {
            size_t j = 0;
            size_t i = strlen(chResult);
            int t = !(i % 2) ? 1 : 0;
            int k = 0;
            strcpy(convertedValue, chResult);
            for (j = i - 1; j > (i / 2 - t); j--)
            {
                char ch  = chResult[j];
                chResult[j] = chResult[k];
                chResult[k++] = ch;
            }
            strcpy(convertedValue, chResult);
            *err = ERROR_CONVERTBASE_OK;
        }
        else
        {
            *err = ERROR_CONVERTBASE_ALLOCATION;
            return NULL;
        }
    }

    if (*err == ERROR_CONVERTBASE_OK)
    {
        size_t lenConvertedValue = strlen(convertedValue);
        if ((nbDigits > lenConvertedValue) && (nbDigits > 0))
        {
            size_t i = 0;
            char *tmp = (char*)MALLOC(sizeof(char) * (nbDigits + 1));
            if (tmp)
            {
                for (i = 0; i < nbDigits - lenConvertedValue; i++)
                {
                    tmp[i] = '0';
                }
                tmp[i] = 0;
                strcat(tmp, convertedValue);
                FREE(convertedValue);
                convertedValue = tmp;
            }
        }
    }

    return convertedValue;
}
/*--------------------------------------------------------------------------*/
char **convertMatrixOfDec2Base(const double* dValues, int mn,
                               int numberbase, unsigned int nbDigits,
                               error_convertbase *err)
{
    char **convertedValues = NULL;
    int i = 0;
    double maxVal = 0.;

    for (i = 0; i < mn; i++)
    {
        long long int iValue = (long long int) dValues[i];

        /* search max value */
        if (dValues[i] > maxVal)
        {
            maxVal = dValues[i];
        }

        /* check if it is an integer value */
        if (dValues[i] != (double)iValue)
        {
            *err = ERROR_CONVERTBASE_NOT_INTEGER_VALUE;
            return NULL;
        }

        /* check if it is in the good interval */
        if ((dValues[i] < 0) || (dValues[i] > pow(2, 52)))
        {
            *err = ERROR_CONVERTBASE_NOT_IN_INTERVAL;
            return NULL;
        }
    }

    if ((mn > 1) && (numberbase == 2)) /* Only binary base is uniformed about number of digits */
    {
        size_t maxDigits = 0;
        char *maxBaseString = convertDec2Base(maxVal, numberbase, nbDigits, err);
        if (maxBaseString)
        {
            maxDigits = strlen(maxBaseString);
            FREE(maxBaseString);
            if (maxDigits > nbDigits)
            {
                nbDigits = (unsigned int)maxDigits;
            }
        }
    }

    convertedValues = (char **)MALLOC(sizeof(char*) * (mn));
    if (convertedValues)
    {
        for (i = 0; i < mn; i++)
        {
            convertedValues[i] = convertDec2Base(dValues[i], numberbase, nbDigits, err);
            if (*err)
            {
                freeArrayOfString(convertedValues, mn);
                return NULL;
            }
        }
    }
    else
    {
        *err = ERROR_CONVERTBASE_ALLOCATION;
    }
    return convertedValues;
}
/*--------------------------------------------------------------------------*/
