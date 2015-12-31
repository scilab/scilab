/*
* ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009-2010 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <ctype.h>
#include <string.h>
#include "pathconvert.h"
#include "sci_malloc.h"
#include "expandPathVariable.h"
#include "splitpath.h"
#include "charEncoding.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
#define CYGWINSTART "/cygdrive/"
/*--------------------------------------------------------------------------*/
static char* cygwintowindowspath(const char* cygwinpath, BOOL *bConverted);
static char* windowstocygwinpath(const char* windowspath, BOOL *bConverted);
/*--------------------------------------------------------------------------*/
char *pathconvert(const char* path, BOOL flagtrail, BOOL flagexpand, PathConvertType PType)
{
    char* convertedPath = NULL;
    if (path)
    {
        BOOL bConvCyg = FALSE;
        char* expandedPath = NULL;
        PathConvertType PTypelocal = PType;
        int i = 0;

        if (PType == AUTO_STYLE)
        {
#ifdef _MSC_VER
            PTypelocal = WINDOWS_STYLE;
#else
            PTypelocal = UNIX_STYLE;
#endif
        }

        if (flagexpand)
        {
            expandedPath = expandPathVariable(path);
        }
        else
        {
            expandedPath = (char*)MALLOC(sizeof(char) * ((int)strlen(path) + 1));
            strcpy(expandedPath, path);
        }

        if (PTypelocal == WINDOWS_STYLE)
        {
            convertedPath = cygwintowindowspath(expandedPath, &bConvCyg);
        }
        else
        {
            convertedPath = windowstocygwinpath(expandedPath, &bConvCyg);
        }

        FREE(expandedPath);

        if (convertedPath)
        {
            if (flagtrail)
            {
                int currentLen = (int)strlen(convertedPath);
                if ( (convertedPath[currentLen - 1] != '/') && (convertedPath[currentLen - 1] != '\\') )
                {
                    convertedPath = (char*)REALLOC(convertedPath, (currentLen + 2) * sizeof(char));
                    if (PTypelocal == WINDOWS_STYLE)
                    {
                        strcat(convertedPath, "\\");
                    }
                    else
                    {
                        strcat(convertedPath, "/");
                    }
                }
            }
            else
            {
                int currentLen = (int) strlen(convertedPath);
                if ( (convertedPath[currentLen - 1] == '/') || (convertedPath[currentLen - 1] == '\\') )
                {
                    convertedPath[currentLen - 1] = '\0';
                }
            }

            for (i = 0; i < (int)strlen(convertedPath); i++)
            {
                if (PTypelocal == WINDOWS_STYLE)
                {
                    if (convertedPath[i] == '/')
                    {
                        convertedPath[i] = '\\';
                    }
                }
                else
                {
                    if (convertedPath[i] == '\\')
                    {
                        convertedPath[i] = '/';
                    }
                }
            }
        }
    }
    return convertedPath;
}
/*--------------------------------------------------------------------------*/
static char* cygwintowindowspath(const char* cygwinpath, BOOL *bConverted)
{
    char* windowspath = NULL;
    *bConverted	= FALSE;
    if (cygwinpath)
    {
        int lenBegin = (int)strlen(CYGWINSTART);
        int lenPath = (int)strlen(cygwinpath);

        windowspath = (char*)MALLOC(sizeof(char) * (lenPath + 1));

        if ( strncmp(cygwinpath, CYGWINSTART, lenBegin) == 0)
        {
            if (lenBegin == lenPath)
            {
                if (windowspath)
                {
                    strcpy(windowspath, cygwinpath);
                    *bConverted	= FALSE;
                }
            }
            else
            {
                if ( (lenPath > lenBegin) && isalpha(cygwinpath[lenBegin]) )
                {
                    if ( (lenPath >= lenBegin + 1) &&
                            ((cygwinpath[lenBegin + 1] == '/') || (cygwinpath[lenBegin + 1] == '\\')) )
                    {
                        windowspath[0] = cygwinpath[lenBegin];
                        windowspath[1] = ':';
                        windowspath[2] = '\0';

                        if (lenPath > lenBegin + 1)
                        {
                            strcat(windowspath, &cygwinpath[lenBegin + 1]);
                        }
                        *bConverted	= TRUE;
                    }
                    else
                    {
                        if (windowspath)
                        {
                            strcpy(windowspath, cygwinpath);
                        }
                        *bConverted	= FALSE;
                    }
                }
                else
                {
                    if (windowspath)
                    {
                        strcpy(windowspath, cygwinpath);
                    }
                    *bConverted	= FALSE;
                }
            }
        }
        else
        {
            if (windowspath)
            {
                strcpy(windowspath, cygwinpath);
            }
            *bConverted	= FALSE;
        }
    }
    return windowspath;
}
/*--------------------------------------------------------------------------*/
static char* windowstocygwinpath(const char* windowspath, BOOL *bConverted)
{
    char* cygwinpath = NULL;
    *bConverted = FALSE;

    if (windowspath)
    {
        char *drv = (char*)MALLOC(sizeof(char) * ((int)strlen(windowspath) + 1));
        char* dir = (char*)MALLOC(sizeof(char) * ((int)strlen(windowspath) + 1));
        char* name = (char*)MALLOC(sizeof(char) * ((int)strlen(windowspath) + 1));
        char* ext = (char*)MALLOC(sizeof(char) * ((int)strlen(windowspath) + 1));

        splitpath(windowspath, FALSE, drv, dir, name, ext);
        if (strcmp(drv, "") != 0)
        {
            int len = (int)strlen(CYGWINSTART) + (int)strlen(drv) +
                      (int)strlen(dir) + (int)strlen(name) +
                      (int)strlen(ext) + 3;

            cygwinpath = (char*)MALLOC(sizeof(char) * len);
            if (cygwinpath)
            {
                strcpy(cygwinpath, CYGWINSTART);
                strcat(cygwinpath, drv);
                len = (int)strlen(cygwinpath);
                if (cygwinpath[len - 1] == ':')
                {
                    cygwinpath[len - 1] = '\0';
                }
                if (strcmp(dir, "") != 0)
                {
                    strcat(cygwinpath, dir);

                    if (strcmp(name, "") != 0)
                    {
                        strcat(cygwinpath, name);
                        if (strcmp(ext, "") != 0)
                        {
                            strcat(cygwinpath, ext);
                        }
                    }
                }
            }
            *bConverted = TRUE;
        }
        else
        {
            cygwinpath = (char*)MALLOC(sizeof(char) * ((int)strlen(windowspath) + 1));
            if (cygwinpath)
            {
                strcpy(cygwinpath, windowspath);
            }
        }

        FREE(drv);
        FREE(dir);
        FREE(name);
        FREE(ext);
    }
    return cygwinpath;
}
/*--------------------------------------------------------------------------*/
