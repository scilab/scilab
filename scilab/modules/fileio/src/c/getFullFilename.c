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
#include "getFullFilename.h"
#include "charEncoding.h"
#include "sci_malloc.h"
#include "splitpath.h"
#include "PATH_MAX.h"
#include "scicurdir.h"
#include "isdir.h"
#include "fullpath.h"
/*--------------------------------------------------------------------------*/
wchar_t *getFullFilenameW(const wchar_t* FilenameInput)
{
    wchar_t *pStwcFullFilename = NULL;

    pStwcFullFilename = (wchar_t *)MALLOC(sizeof(wchar_t) * (PATH_MAX * 2));
    if (pStwcFullFilename)
    {
        int i = 0;
        int lenPath = 0;

        wchar_t wcDrv[PATH_MAX * 2];
        wchar_t wcDir[PATH_MAX * 2];
        wchar_t wcName[PATH_MAX * 2];
        wchar_t wcExt[PATH_MAX * 2];

        wchar_t *wcNameExt = (wchar_t *)MALLOC(sizeof(wchar_t) * (PATH_MAX * 2));
        wchar_t *wcPath = (wchar_t *)MALLOC(sizeof(wchar_t) * (PATH_MAX * 2));
        wchar_t *wcTmp = NULL;

        if (wcNameExt == NULL || wcPath == NULL)
        {
            FREE(pStwcFullFilename);
            if (wcNameExt)
            {
                FREE(wcNameExt);
            }
            if (wcPath)
            {
                FREE(wcPath);
            }
            return NULL;
        }
        splitpathW(FilenameInput, TRUE, wcDrv, wcDir,  wcName, wcExt);

        wcscpy(wcNameExt, wcName);
        wcscat(wcNameExt, wcExt);

        wcscpy(wcPath, wcDrv);
        wcscat(wcPath, wcDir);

        if (wcscmp(wcPath, L"") == 0)
        {
            int ierr = 0;
            wchar_t *wcCurrentDir = scigetcwdW(&ierr);
            if (ierr == 0)
            {
                wcscpy(wcPath, wcCurrentDir);
            }
            if (wcCurrentDir)
            {
                FREE(wcCurrentDir);
                wcCurrentDir = NULL;
            }
        }

        wcTmp = (wchar_t*)MALLOC(sizeof(wchar_t) * (PATH_MAX * 2));
        if (wcTmp)
        {
            get_full_pathW(wcTmp, (const wchar_t*)wcPath, PATH_MAX * 2);
            wcscpy(wcPath, wcTmp);
            FREE(wcTmp);
            wcTmp = NULL;
        }

        lenPath = (int)wcslen(wcPath);
        if (lenPath - 1 >= 0)
        {
            if ( (wcPath[lenPath - 1 ] != L'/') && (wcPath[lenPath - 1 ] != L'\\') )
            {
                wcscat(wcPath, L"/");
                lenPath = (int)wcslen(wcPath);
            }
        }

        for ( i = 0; i < lenPath; i++)
        {
#ifdef _MSC_VER
            if (wcPath[i] == L'/')
            {
                wcPath[i] = L'\\';
            }
#else
            if (wcPath[i] == L'\\')
            {
                wcPath[i] = L'/';
            }
#endif
        }

        wcscpy(pStwcFullFilename, wcPath);
        wcscat(pStwcFullFilename, wcNameExt);

        FREE(wcNameExt);
        FREE(wcPath);
    }
    return pStwcFullFilename;

}
/*--------------------------------------------------------------------------*/
char *getFullFilename(const char* Filename)
{
    char *pStFullFilename = NULL;
    if (Filename)
    {
        wchar_t *wcFilename = to_wide_string(Filename);
        if (wcFilename)
        {
            wchar_t *pStwcFullFilename = getFullFilenameW(wcFilename);
            FREE(wcFilename);
            if (pStwcFullFilename)
            {
                pStFullFilename = wide_string_to_UTF8(pStwcFullFilename);
                FREE(pStwcFullFilename);
            }
        }
    }

    return pStFullFilename;
}
/*--------------------------------------------------------------------------*/
