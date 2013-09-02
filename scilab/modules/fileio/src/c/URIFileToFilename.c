/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * ...
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "URIFileToFilename.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
#if _MSC_VER
#define strnicmp _strnicmp
#else
#define strnicmp strncasecmp
#endif
/*--------------------------------------------------------------------------*/
#define URI_BEGIN "file://"
/*--------------------------------------------------------------------------*/
char *URIFileToFilename(char *uri)
{
    char *filename = NULL;

    if (uri)
    {
        if (isURIFile(uri))
        {
            int pos = (int) strlen(URI_BEGIN);
            filename = strdup(&uri[pos]);
        }
        else
        {
            filename = strdup(uri);
        }
    }
    return filename;
}
/*--------------------------------------------------------------------------*/
BOOL isURIFile(char *uri)
{
    BOOL bOK = FALSE;
    if (uri)
    {
        if (strlen(uri) > strlen(URI_BEGIN))
        {
            if ( strnicmp(uri, URI_BEGIN, strlen(URI_BEGIN)) == 0)
            {
                bOK = TRUE;
            }
        }
    }
    return bOK;
}
/*--------------------------------------------------------------------------*/
