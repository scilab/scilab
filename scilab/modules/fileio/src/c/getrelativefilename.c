/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Pierre MARECHAL
* Copyright (C) 2011 - Digiteo - Cedric DELAMARRE
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
#include <ctype.h>
#include <string.h>
#include "machine.h" /* DIR_SEPARATOR */
#include "getrelativefilename.h"
#include "sci_malloc.h"
#include "PATH_MAX.h"
#include "strsubst.h"
/* ================================================================================== */
static char *normalizeFileSeparator(const char *path);
static wchar_t *normalizeFileSeparatorW(const wchar_t *path);
/* ================================================================================== */
// getrelativefilename
//
// Given the absolute current directory and an absolute file name, returns a relative file name.
// For example, if the current directory is C:\foo\bar and the filename C:\foo\whee\text.txt is given,
// GetRelativeFilename will return ..\whee\text.txt.
/* ================================================================================== */
char* getrelativefilename(char *currentDirectory, char *absoluteFilename)
{
    int afMarker = 0, rfMarker = 0;
    int cdLen = 0, afLen = 0;
    int i = 0;
    int levels = 0;
    char *relativeFilename = (char*)MALLOC(PATH_MAX * sizeof(char));
    char *_currentDirectory = normalizeFileSeparator(currentDirectory);
    char *_absoluteFilename = normalizeFileSeparator(absoluteFilename);

    cdLen = (int)strlen(_currentDirectory);
    afLen = (int)strlen(_absoluteFilename);

    // make sure the names are not too short
    if ( cdLen < ABSOLUTE_NAME_START + 1 || afLen < ABSOLUTE_NAME_START + 1)
    {
        // fix bug 2181
        strcpy(relativeFilename, _absoluteFilename);
        FREE(_currentDirectory);
        FREE(_absoluteFilename);
        return relativeFilename;
    }

    // Handle DOS names that are on different drives:
    if (tolower(_currentDirectory[0]) != tolower(_absoluteFilename[0]))
    {
        // not on the same drive, so only absolute filename will do
        strcpy(relativeFilename, _absoluteFilename);
        FREE(_currentDirectory);
        FREE(_absoluteFilename);
        return relativeFilename;
    }

    // they are on the same drive, find out how much of the current directory
    // is in the absolute filename
    i = ABSOLUTE_NAME_START;

#if defined(_MSC_VER)
    while (i < afLen && i < cdLen && tolower(_currentDirectory[i]) == tolower(_absoluteFilename[i]) )
    {
        i++;
    }
#else
    while (i < afLen && i < cdLen && _currentDirectory[i] == _absoluteFilename[i])
    {
        i++;
    }
#endif

    if (i == cdLen && (_absoluteFilename[i] == DIR_SEPARATOR[0] || _absoluteFilename[i - 1] == DIR_SEPARATOR[0]))
    {
        // the whole current directory name is in the file name,
        // so we just trim off the current directory name to get the
        // current file name.
        if (_absoluteFilename[i] == DIR_SEPARATOR[0])
        {
            // a directory name might have a trailing slash but a relative
            // file name should not have a leading one...
            i++;
        }

        strcpy(relativeFilename, &_absoluteFilename[i]);
        FREE(_currentDirectory);
        FREE(_absoluteFilename);
        return relativeFilename;
    }

    // The file is not in a child directory of the current directory, so we
    // need to step back the appropriate number of parent directories by
    // using "..\"s.  First find out how many levels deeper we are than the
    // common directory
    afMarker = i;
    levels = 1;

    // count the number of directory levels we have to go up to get to the
    // common directory
    while (i < cdLen)
    {
        i++;
        if (_currentDirectory[i] == DIR_SEPARATOR[0])
        {
            // make sure it's not a trailing slash
            i++;
            if (_currentDirectory[i] != '\0')
            {
                levels++;
            }
        }
    }

    // move the absolute filename marker back to the start of the directory name
    // that it has stopped in.
    while (afMarker > 0 && _absoluteFilename[afMarker - 1] != DIR_SEPARATOR[0])
    {
        afMarker--;
    }

    // check that the result will not be too long
    if (levels * 3 + afLen - afMarker > PATH_MAX)
    {
        FREE(relativeFilename);
        FREE(_currentDirectory);
        FREE(_absoluteFilename);
        return NULL;
    }

    // add the appropriate number of "..\"s.
    rfMarker = 0;
    for (i = 0; i < levels; i++)
    {
        relativeFilename[rfMarker++] = '.';
        relativeFilename[rfMarker++] = '.';
        relativeFilename[rfMarker++] = DIR_SEPARATOR[0];
    }

    // copy the rest of the filename into the result string
    strcpy(&relativeFilename[rfMarker], &_absoluteFilename[afMarker]);

    FREE(_currentDirectory);
    FREE(_absoluteFilename);
    return relativeFilename;
}

wchar_t* getrelativefilenameW(wchar_t *currentDirectory, wchar_t *absoluteFilename)
{
    // declarations - put here so this should work in a C compiler
    int afMarker = 0, rfMarker = 0;
    int cdLen = 0, afLen = 0;
    int i = 0;
    int levels = 0;
    wchar_t *relativeFilename = (wchar_t*)MALLOC(PATH_MAX * sizeof(wchar_t));
    wchar_t *_currentDirectory = normalizeFileSeparatorW(currentDirectory);
    wchar_t *_absoluteFilename = normalizeFileSeparatorW(absoluteFilename);

    cdLen = (int)wcslen(_currentDirectory);
    afLen = (int)wcslen(_absoluteFilename);

    // make sure the names are not too short
    if ( cdLen < ABSOLUTE_NAME_START + 1 || afLen < ABSOLUTE_NAME_START + 1)
    {
        // fix bug 2181
        wcscpy(relativeFilename, _absoluteFilename);
        return relativeFilename;
    }

    // Handle DOS names that are on different drives:
    if (tolower(_currentDirectory[0]) != tolower(_absoluteFilename[0]))
    {
        // not on the same drive, so only absolute filename will do
        wcscpy(relativeFilename, _absoluteFilename);
        return relativeFilename;
    }

    // they are on the same drive, find out how much of the current directory
    // is in the absolute filename
    i = ABSOLUTE_NAME_START;

#if defined(_MSC_VER)
    while (i < afLen && i < cdLen && tolower(_currentDirectory[i]) == tolower(_absoluteFilename[i]) )
    {
        i++;
    }
#else
    while (i < afLen && i < cdLen && _currentDirectory[i] == _absoluteFilename[i])
    {
        i++;
    }
#endif

    if (i == cdLen && (_absoluteFilename[i] == DIR_SEPARATOR[0] || _absoluteFilename[i - 1] == DIR_SEPARATOR[0]))
    {
        // the whole current directory name is in the file name,
        // so we just trim off the current directory name to get the
        // current file name.
        if (_absoluteFilename[i] == DIR_SEPARATOR[0])
        {
            // a directory name might have a trailing slash but a relative
            // file name should not have a leading one...
            i++;
        }

        wcscpy(relativeFilename, &_absoluteFilename[i]);
        return relativeFilename;
    }

    // The file is not in a child directory of the current directory, so we
    // need to step back the appropriate number of parent directories by
    // using "..\"s.  First find out how many levels deeper we are than the
    // common directory
    afMarker = i;
    levels = 1;

    // count the number of directory levels we have to go up to get to the
    // common directory
    while (i < cdLen)
    {
        i++;
        if (_currentDirectory[i] == DIR_SEPARATOR[0])
        {
            // make sure it's not a trailing slash
            i++;
            if (currentDirectory[i] != '\0')
            {
                levels++;
            }
        }
    }

    // move the absolute filename marker back to the start of the directory name
    // that it has stopped in.
    while (afMarker > 0 && _absoluteFilename[afMarker - 1] != DIR_SEPARATOR[0])
    {
        afMarker--;
    }

    // check that the result will not be too long
    if (levels * 3 + afLen - afMarker > PATH_MAX)
    {
        FREE(relativeFilename);
        return NULL;
    }

    // add the appropriate number of "..\"s.
    rfMarker = 0;
    for (i = 0; i < levels; i++)
    {
        relativeFilename[rfMarker++] = '.';
        relativeFilename[rfMarker++] = '.';
        relativeFilename[rfMarker++] = DIR_SEPARATOR[0];
    }

    // copy the rest of the filename into the result string
    wcscpy(&relativeFilename[rfMarker], &_absoluteFilename[afMarker]);

    return relativeFilename;
}

/* ================================================================================== */
char *normalizeFileSeparator(const char *path)
{
#define WINDOWS_FILESEPARATOR "\\"
#define OTHERS_FILESEPARATOR "/"

    char *normalizedPath = NULL;
    if (path)
    {
#ifdef _MSC_VER
        normalizedPath = strsub((char*)path, OTHERS_FILESEPARATOR, DIR_SEPARATOR);
#else
        normalizedPath = strsub((char*)path, WINDOWS_FILESEPARATOR, DIR_SEPARATOR);
#endif
    }
    return normalizedPath;
}

wchar_t *normalizeFileSeparatorW(const wchar_t *path)
{
    // TODO : Implement strsubW to have same behaviour.
    return (wchar_t*)path;
}
/* ================================================================================== */

