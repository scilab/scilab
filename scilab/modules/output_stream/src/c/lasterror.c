/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2010 - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/ 
#include <string.h>
#include <stdlib.h>
#include "MALLOC.h"
#include "lasterror.h"
#include "stack-def.h"
#include "freeArrayOfString.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "strsubst.h"
/*--------------------------------------------------------------------------*/
/* max numbers of lines in "error" buffer for compatibility */
#define MAX_ERROR_MESSAGE_NB_LINES  20
#define NO_ERROR_VALUE 0
#define NO_ERROR_MESSAGE ""
#define NO_ERROR_MESSAGE_NB_LINES 0
#define NO_ERROR_LINEPOSITION 0
#define NO_ERROR_FUNCTIONNAME ""
#define CR "\n"
#define LF "\r"
#define EMPTY_STRING ""
/*--------------------------------------------------------------------------*/
static int iLastErrorValue = NO_ERROR_VALUE;
static char **strLastErrorMessage = NULL;
static int iLastErrorMessageNbLines = NO_ERROR_MESSAGE_NB_LINES;
static int iLastErrorLinePostion = NO_ERROR_LINEPOSITION;
static char strLastErrorFunctionName[nlgh + 1] = NO_ERROR_FUNCTIONNAME;
/*--------------------------------------------------------------------------*/
static void allocLastErrorMessage(void)
{
    strLastErrorMessage = (char **)MALLOC(sizeof(char*) * MAX_ERROR_MESSAGE_NB_LINES);
    iLastErrorMessageNbLines = NO_ERROR_MESSAGE_NB_LINES;
}
/*--------------------------------------------------------------------------*/
static void freeLastErrorMessage(void)
{
    if (strLastErrorMessage)
    {
        freeArrayOfString(strLastErrorMessage, iLastErrorMessageNbLines);
    }
    iLastErrorMessageNbLines = NO_ERROR_MESSAGE_NB_LINES;
}
/*--------------------------------------------------------------------------*/ 
int setLastError(int iErr, char** strErrorMessage, int iNbLines, int iLinePos, char *strFunctionName)
{
    setLastErrorValue(iErr);

    if (iNbLines == 0)
    {
        setLastErrorMessage(NULL, iNbLines);
    }
    else
    {
        setLastErrorMessage(strErrorMessage, iNbLines);
    }

    setLastErrorLinePosition(iLinePos);
    setLastErrorFunctionName(strFunctionName);
    return 0;
}
/*--------------------------------------------------------------------------*/ 
int clearLastError(void)
{
    return setLastError(NO_ERROR_VALUE, 
        NULL, 
        NO_ERROR_MESSAGE_NB_LINES, 
        NO_ERROR_LINEPOSITION, 
        NO_ERROR_FUNCTIONNAME);
}
/*--------------------------------------------------------------------------*/ 
int setLastErrorMessage(char** strErrorMessage, int iNbLines)
{
    int i = 0;
    if (strLastErrorMessage)
    {
        freeArrayOfString(strLastErrorMessage, iLastErrorMessageNbLines);
    }
    allocLastErrorMessage();

    for (i = 0; i < iNbLines; i++)
    {
        appendStringToLastErrorMessage(strErrorMessage[i]);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/ 
const char** getLastErrorMessage(int *iNbLines)
{
    if (iLastErrorMessageNbLines > 0)
    {
        *iNbLines = iLastErrorMessageNbLines;
        return strLastErrorMessage;
    }
    else
    {
        *iNbLines = 0;
        return NULL;
    }
}
/*--------------------------------------------------------------------------*/ 
int appendStringToLastErrorMessage(char *strErrorMessage)
{
    if (iLastErrorMessageNbLines >= MAX_ERROR_MESSAGE_NB_LINES)
    {
        return 1;
    }
    else
    {
        if (strLastErrorMessage)
        {
            if (strErrorMessage == NULL)
            {
                strLastErrorMessage[iLastErrorMessageNbLines] = strdup(NO_ERROR_MESSAGE);
            }
            else
            {
                char *withoutCR = strsub(strErrorMessage, CR, EMPTY_STRING);
                strLastErrorMessage[iLastErrorMessageNbLines] = strsub(withoutCR, LF, EMPTY_STRING);
                if (withoutCR)
                {
                    FREE(withoutCR);
                    withoutCR = NULL;
                }
            }
            iLastErrorMessageNbLines++;
        }
        else
        {
            return 1;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/ 
int setLastErrorValue(int iVal)
{
    int iPrevValue = iLastErrorValue;
    iLastErrorValue = iVal;
    return iPrevValue;
}
/*--------------------------------------------------------------------------*/ 
int getLastErrorValue(void)
{
    return iLastErrorValue;
}
/*--------------------------------------------------------------------------*/ 
int setLastErrorLinePosition(int iLinePosition)
{
    int iPrevLinePosition = iLastErrorLinePostion;
    iLastErrorLinePostion = iLinePosition;
    return iPrevLinePosition;
}
/*--------------------------------------------------------------------------*/ 
int getLastErrorLinePosition(void)
{
    return iLastErrorLinePostion;
}
/*--------------------------------------------------------------------------*/ 
int setLastErrorFunctionName(char *strFunctionName)
{
    if (strFunctionName)
    {
        if (strlen(strFunctionName) > nlgh)
        {
            strncpy(strLastErrorFunctionName, strFunctionName, nlgh);
            strLastErrorFunctionName[nlgh] = 0;
        }
        else
        {
            strcpy(strLastErrorFunctionName, strFunctionName);
        }
    }
    else
    {
        strcpy(strLastErrorFunctionName, NO_ERROR_FUNCTIONNAME);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/ 
const char *getLastErrorFunctionName(void)
{
    return strLastErrorFunctionName;
}
/*--------------------------------------------------------------------------*/ 
