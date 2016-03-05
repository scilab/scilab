/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
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
#include "BOOL.h"
#include "dynlib_scilab_windows.h"
/*--------------------------------------------------------------------------*/
#define UNIX_SEPATATOR '/'
#define WINDOWS_SEPATATOR '\\'
/*--------------------------------------------------------------------------*/
static BOOL convertSlash(char *path_in, char *path_out, BOOL slashToAntislash);
/*--------------------------------------------------------------------------*/
SCILAB_WINDOWS_IMPEXP BOOL slashToAntislash(char *pathunix, char *pathwindows)
{
    return convertSlash(pathunix, pathwindows, TRUE);
}
/*--------------------------------------------------------------------------*/
SCILAB_WINDOWS_IMPEXP BOOL AntislashToSlash(char *pathwindows, char *pathunix)
{
    return convertSlash(pathwindows, pathunix, FALSE);
}
/*--------------------------------------------------------------------------*/
static BOOL convertSlash(char *path_in, char *path_out, BOOL slashToAntislash)
{
    BOOL bOK = FALSE;
    if ( (path_in) && (path_out) )
    {
        int i = 0;
        int len_path_out = 0;
        strcpy(path_out, path_in);
        len_path_out = (int)strlen(path_out);

        for (i = 0; i < len_path_out; i++)
        {
            if ( slashToAntislash )
            {
                if (path_in[i] == UNIX_SEPATATOR)
                {
                    path_out[i] = WINDOWS_SEPATATOR;
                    bOK = TRUE;
                }
            }
            else
            {
                if (path_in[i] == WINDOWS_SEPATATOR)
                {
                    path_out[i] = UNIX_SEPATATOR;
                    bOK = TRUE;
                }
            }
        }
    }
    else
    {
        return FALSE;
    }

    return bOK;
}
/*--------------------------------------------------------------------------*/
