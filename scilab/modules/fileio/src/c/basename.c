/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#include "basename.h"
#include "sci_malloc.h"
#include "expandPathVariable.h"
#include "splitpath.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
wchar_t *basenameW(wchar_t *wcfullfilename, BOOL bExpand)
{
    wchar_t *basename_str = NULL;
    if (wcfullfilename)
    {
        wchar_t *expandedPath = expandPathVariableW(wcfullfilename);
        if (expandedPath)
        {
            wchar_t *wcdrv = (wchar_t*)MALLOC(sizeof(wchar_t) * ((int)wcslen(expandedPath) + 1));
            wchar_t* wcdir = (wchar_t*)MALLOC(sizeof(wchar_t) * ((int)wcslen(expandedPath) + 1));
            wchar_t* wcname = (wchar_t*) MALLOC(sizeof(wchar_t) * ((int)wcslen(expandedPath) + 1));
            wchar_t* wcext = (wchar_t*)MALLOC(sizeof(wchar_t) * ((int)wcslen(expandedPath) + 1));

            splitpathW(expandedPath, bExpand, wcdrv, wcdir, wcname, wcext);

            if (wcname)
            {
                basename_str = wcname;
            }

            if (wcdrv)
            {
                FREE(wcdrv);
                wcdrv = NULL;
            }
            if (wcdir)
            {
                FREE(wcdir);
                wcdir = NULL;
            }
            if (wcext)
            {
                FREE(wcext);
                wcext = NULL;
            }

            FREE(expandedPath);
            expandedPath = NULL;
        }
    }
    return basename_str;
}
