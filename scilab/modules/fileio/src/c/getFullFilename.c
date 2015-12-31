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
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "getFullFilename.h"
#include "charEncoding.h"
#include "sci_malloc.h"
#include "splitpath.h"
#include "PATH_MAX.h"
#include "scicurdir.h"
#include "isdir.h"
#include "fullpath.h"
/*--------------------------------------------------------------------------*/
char *getFullFilename(const char* Filename)
{
    char *pStFullFilename = NULL;

    pStFullFilename = (char *)MALLOC(sizeof(char) * (PATH_MAX * 2));
    if (pStFullFilename)
    {
        int i = 0;
        int lenPath = 0;

        char Drv[PATH_MAX * 2];
        char Dir[PATH_MAX * 2];
        char Name[PATH_MAX * 2];
        char Ext[PATH_MAX * 2];

        char *NameExt = (char *)MALLOC(sizeof(char) * (PATH_MAX * 2));
        char *Path = (char *)MALLOC(sizeof(char) * (PATH_MAX * 2));
        char *Tmp = NULL;

        if (NameExt == NULL || Path == NULL)
        {
            FREE(pStFullFilename);
            if (NameExt)
            {
                FREE(NameExt);
            }
            if (Path)
            {
                FREE(Path);
            }
            return NULL;
        }
        splitpath(Filename, TRUE, Drv, Dir, Name, Ext);

        strcpy(NameExt, Name);
        strcat(NameExt, Ext);

        strcpy(Path, Drv);
        strcat(Path, Dir);

        if (strcmp(Path, "") == 0)
        {
            int ierr = 0;
            char *CurrentDir = scigetcwd(&ierr);
            if (ierr == 0)
            {
                strcpy(Path, CurrentDir);
            }
            if (CurrentDir)
            {
                FREE(CurrentDir);
                CurrentDir = NULL;
            }
        }

        Tmp = (char*)MALLOC(sizeof(char) * (PATH_MAX * 2));
        if (Tmp)
        {
            get_full_path(Tmp, (const char*)Path, PATH_MAX * 2);
            strcpy(Path, Tmp);
            FREE(Tmp);
            Tmp = NULL;
        }

        lenPath = (int)strlen(Path);
        if (lenPath - 1 >= 0)
        {
            if ( (Path[lenPath - 1 ] != '/') && (Path[lenPath - 1 ] != '\\') )
            {
                strcat(Path, "/");
                lenPath = (int)strlen(Path);
            }
        }

        for ( i = 0; i < lenPath; i++)
        {
#ifdef _MSC_VER
            if (Path[i] == '/')
            {
                Path[i] = '\\';
            }
#else
            if (Path[i] == '\\')
            {
                Path[i] = '/';
            }
#endif
        }

        strcpy(pStFullFilename, Path);
        strcat(pStFullFilename, NameExt);

        FREE(NameExt);
        FREE(Path);
    }
    return pStFullFilename;

}
/*--------------------------------------------------------------------------*/
