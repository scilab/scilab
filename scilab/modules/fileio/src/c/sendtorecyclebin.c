/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
/*--------------------------------------------------------------------------*/
#include <Windows.h>
#include <Shellapi.h>
#include "sendtorecyclebin.h"
/*--------------------------------------------------------------------------*/
BOOL sendToRecycleBin(const char* filename)
{
    BOOL bOK = FALSE;

    wchar_t* wfilename = to_wide_string((char*)filename);
    if (wfilename)
    {
        bOK = sendToRecycleBinW(wfilename);
        FREE(wfilename);
        wfilename = NULL;
    }
    return bOK;
}
/*--------------------------------------------------------------------------*/
BOOL sendToRecycleBinW(const wchar_t* wfilename)
{
    BOOL bOK = FALSE;
    if (wfilename)
    {
        SHFILEOPSTRUCTW opFile;
        size_t len = wcslen(wfilename) + 2;
        /* we add 00 at the end of filename */
        /* required by SHFileOperationW */
        wchar_t* csrc = (wchar_t*)CALLOC(len, sizeof(wchar_t));
        if (csrc)
        {
            wcscpy(csrc, wfilename);
            /* we add 00 at the end of filename */
            csrc[len - 1] = csrc[len - 2] = 0;
            ZeroMemory(&opFile, sizeof(opFile));
            opFile.wFunc = FO_DELETE;
            opFile.pFrom = csrc;
            opFile.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_SILENT;
            bOK = SHFileOperationW(&opFile) != 0 ? FALSE : TRUE;
            FREE(csrc);
            csrc = NULL;
        }
    }
    return bOK;
}
/*--------------------------------------------------------------------------*/