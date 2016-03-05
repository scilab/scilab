/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#include <string.h> /* strcmp */
#include <stdio.h> /* sprintf */
#include <stdlib.h> /* qsort */
#include "getfilesdictionary.h"
#include "PATH_MAX.h"
#include "scicurdir.h" /* scigetcwd */
#include "findfiles.h" /* findfiles */
#include "sci_malloc.h"
#include "expandPathVariable.h"
#include "machine.h"
#include "isdir.h"
/*--------------------------------------------------------------------------*/
static void mysplitpath(char *composite,  char *path,  char *fname);
static char **addPath(char **dictionary, int sizearray, char *path);
static char **addDirSeparator(char **dictionary, int sizearray, char *path);
/*--------------------------------------------------------------------------*/
static int cmpfiles( const void *a , const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b );
}
/*--------------------------------------------------------------------------*/
char **getfilesdictionary(char *somechars, int *sizearray, BOOL fullpath)
{
    char **dictionary = NULL;

    if (somechars)
    {
        int sizeListReturned = 0;
        char path[PATH_MAX];

        char filespec[PATH_MAX];

        char pathname[PATH_MAX];
        char filename[PATH_MAX];

        char *pathextended = NULL;


        mysplitpath(somechars, pathname, filename);

        if ( strcmp(pathname, "") == 0 )
        {
            /* current path */
            int ierr = 0;
            char *currentpath = scigetcwd(&ierr);
            if (currentpath)
            {
                strcpy(path, currentpath);
                strcat(path, DIR_SEPARATOR);
                FREE(currentpath);
                currentpath = NULL;
            }
        }
        else
        {
            /* we have a path */
            strcpy(path, pathname);
        }

        if ( strcmp(filename, "") == 0 )
        {
            /* no filename */
            strcpy(filespec, "*");
        }
        else
        {
            /* we have the beginning of a filename */
            sprintf(filespec, "%s*", filename);
        }

        pathextended = expandPathVariable(path);

        if (pathextended)
        {
            dictionary = findfiles(pathextended, filespec, &sizeListReturned, FALSE);
            FREE(pathextended);
            pathextended = NULL;
        }

        if (fullpath)
        {
            dictionary = addPath(dictionary, sizeListReturned, path);
        }

        dictionary = addDirSeparator(dictionary, sizeListReturned, path);

        *sizearray = sizeListReturned;

        /* Add a NULL element at the end (to get number of items from JNI) */
        if (dictionary != NULL)
        {
            dictionary = (char**)REALLOC(dictionary, sizeof(char*) * (sizeListReturned + 1));
            dictionary[sizeListReturned] = NULL;
            qsort(dictionary, *sizearray, sizeof dictionary[0], cmpfiles);
        }
    }
    else
    {
        *sizearray = 0;
    }
    return dictionary;
}
/*--------------------------------------------------------------------------*/
static void mysplitpath(char *composite,  char *path,  char *fname)
{
    if (composite && path && fname)
    {
        char *lastslash = NULL;
        char *p2 = NULL;

        lastslash = NULL;
        p2 = composite;

        while (*p2)
        {
#ifdef _MSC_VER
            if ( (*p2 == '/') || (*p2 == '\\') )
            {
                lastslash = p2;
            }
#else
            if (*p2 == '/')
            {
                lastslash = p2;
            }
#endif
            p2++;
        }

        if (lastslash != NULL)
        {
            strncpy(path, composite, 1 + (int)(lastslash - composite));
            path[1 + (int)(lastslash - composite)] = '\0';
            strcpy(fname, lastslash + 1);
        }
        else
        {
            strcpy(path, "");
            strcpy(fname, composite);
        }
    }
}
/*--------------------------------------------------------------------------*/
static char **addPath(char **dictionary, int sizearray, char *path)
{
    int i = 0;
    for (i = 0; i < sizearray; i++)
    {
        char *newPath = NULL;
        int newlength = (int)(strlen(dictionary[i]) + strlen(path) + 1);
        newPath = (char *)MALLOC(sizeof(char) * (newlength));
        sprintf(newPath, "%s%s", path, dictionary[i]);
        FREE(dictionary[i]);
        dictionary[i] = newPath;
    }
    return dictionary;
}
/*--------------------------------------------------------------------------*/
static char **addDirSeparator(char **dictionary, int sizearray, char *path)
{
    int i = 0;
    for (i = 0; i < sizearray; i++)
    {
        char *pathextended = NULL;
        char fullpath[PATH_MAX * 2];

        pathextended = expandPathVariable(path);
        if (pathextended)
        {
            strcpy(fullpath, pathextended);
            strcat(fullpath, dictionary[i]);
        }
        else
        {
            strcpy(fullpath, dictionary[i]);
        }

        if ( isdir(fullpath) && (dictionary[i][strlen(dictionary[i]) - 1] != DIR_SEPARATOR[0]) )
        {
            char *newPath = NULL;
            int newlength = (int)(strlen(dictionary[i]) + strlen(DIR_SEPARATOR) + 1);
            newPath = (char *)MALLOC(sizeof(char) * (newlength));
            sprintf(newPath, "%s%s", dictionary[i], DIR_SEPARATOR);
            FREE(dictionary[i]);
            dictionary[i] = newPath;
        }

        if (pathextended)
        {
            FREE(pathextended);
            pathextended = NULL;
        }
    }
    return dictionary;
}
/*--------------------------------------------------------------------------*/
