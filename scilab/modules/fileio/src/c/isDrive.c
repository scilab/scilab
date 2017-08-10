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
/*--------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#endif
#include "isDrive.h"
/*--------------------------------------------------------------------------*/
BOOL isDrive(const char *strname)
{
    BOOL bOK = FALSE;
#ifdef _MSC_VER
    if (strname)
    {
        if ( ((strlen(strname) == 2) || (strlen(strname) == 3)) && (strname[1] == ':') )
        {
            if (strlen(strname) == 3)
            {
                if ( (strname[2] != '\\') && (strname[2] != '/') )
                {
                    return FALSE;
                }
            }

            if ( ( strname[0] >= 'A' && strname[0] <= 'Z' ) || ( strname[0] >= 'a' && strname[0] <= 'z' ) )
            {
                bOK = TRUE;
            }
        }
    }
#endif
    return bOK;
}
/*--------------------------------------------------------------------------*/
BOOL isDriveW(const wchar_t *wcstrname)
{
    BOOL bOK = FALSE;
#ifdef _MSC_VER
    if (wcstrname)
    {
        if ( ((wcslen(wcstrname) == 2) || (wcslen(wcstrname) == 3)) && (wcstrname[1] == L':') )
        {
            if (wcslen(wcstrname) == 3)
            {
                if ( (wcstrname[2] != L'\\') && (wcstrname[2] != L'/') )
                {
                    return FALSE;
                }
            }

            if ( ( wcstrname[0] >= L'A' && wcstrname[0] <= L'Z' ) || ( wcstrname[0] >= L'a' && wcstrname[0] <= L'z' ) )
            {
                bOK = TRUE;
            }
        }
    }
#endif
    return bOK;
}
/*--------------------------------------------------------------------------*/

