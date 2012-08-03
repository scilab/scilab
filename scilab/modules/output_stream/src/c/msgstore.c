/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA
* Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include <string.h>
#include <stdlib.h>
#include "MALLOC.h"
#include "stack-c.h"
#include "msgstore.h"
#include "lasterror.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "freeArrayOfString.h"
#include "strsubst.h"
/*--------------------------------------------------------------------------*/
static char **splitErrorMessage(const char *msg, int *nbLines);
/*--------------------------------------------------------------------------*/
int C2F(linestore)(int *n)
{
    setInternalLastErrorLinePosition(*n);
    return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(funnamestore)(char *str, int *n, int lenstr)
{
    char *functionName = strdup(str);
    if ( (functionName) && (*n >= 0) )
    {
        functionName[*n] = 0;
        setInternalLastErrorFunctionName(functionName);
    }

    if (functionName)
    {
        FREE(functionName);
        functionName = NULL;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(msgstore)(char *str, int *n)
{
    int iRes = 0;

    char **multilines = NULL;
    int nbLines = 0;
    int i = 0;
    char *msg = NULL;
    char *msgTmp = NULL;

    if (str == NULL) return 1;
    msg = strdup(str);
    if (msg)
    {
        msg[*n] = 0;
    }

    // remove duplicate percent bug 9571
    msgTmp = strsub(msg, "%%", "%");
    if (msgTmp)
    {
        FREE(msg);
        msg = msgTmp;
    }
    multilines = splitErrorMessage(msg, &nbLines);
    if (multilines)
    {
        for (i = 0; i < nbLines; i++)
        {
            iRes = appendStringToInternalLastErrorMessage(multilines[i]);
            if (iRes) break;
        }
        freeArrayOfString(multilines, nbLines);
        multilines = NULL;
    }
    else
    {
        iRes = appendStringToInternalLastErrorMessage(msg);
    }

    if (msg)
    {
        FREE(msg);
        msg = NULL;
    }

    return iRes;
}
/*--------------------------------------------------------------------------*/
int GetLastErrorCode(void)
{
    return getInternalLastErrorValue();
}
/*--------------------------------------------------------------------------*/
static char **splitErrorMessage(const char *msg, int *nbLines)
{
    const char* separator = "\n";
    int removeEmptyField = 1;

    size_t sizeSeparator = strlen(separator);
    char** currentSplit  = NULL;
    size_t nbSep = 0;
    size_t nbEmptyField = 0;
    size_t sizeSplit = 0;
    size_t currentSep = 0;
    char* pos = NULL;
    char* currentPos = NULL;
    char *str = strdup(msg);

    *nbLines = 0;

    if (msg == NULL) 
    {
        return NULL;
    }

    pos = str;
    currentPos = str;

    while( pos = strstr( pos, separator ) ) 
    {
        memset( pos, '\0', sizeSeparator );
        if ( pos == str || ( pos != str && !*(pos-1) ) )
        {
            ++nbEmptyField;
        }
        ++nbSep;
        currentPos = (pos += sizeSeparator);
    }
    if ( currentPos == str || ( currentPos != str && !*currentPos ) )
    {
        ++nbEmptyField;
    }

    sizeSplit = (nbSep + 1) - (removeEmptyField?nbEmptyField:0);
    if (!sizeSplit) 
    {
        if (str)
        {
            FREE(str);
            str = NULL;
        }
        return NULL;
    }

    currentSplit = (char **)MALLOC( (sizeSplit) * sizeof(char*) );
    if (!currentSplit) 
    {
        if (str)
        {
            FREE(str);
            str = NULL;
        }
        return NULL;
    }
    pos = str;
    while( currentSep < sizeSplit ) 
    {
        if ( !( removeEmptyField && !*pos ) )
        {
            currentSplit[currentSep++] = strdup(pos);
        }

        while( *pos++ );
        pos += sizeSeparator - 1;
    }

    if (str)
    {
        FREE(str);
        str = NULL;
    }

    *nbLines = (int)sizeSplit;
    return currentSplit;
}
/*--------------------------------------------------------------------------*/
