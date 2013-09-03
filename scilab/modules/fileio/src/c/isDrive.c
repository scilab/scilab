/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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

