/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) INRIA - Allan CORNET
*  Copyright (C) 2010 - DIGITEO - Allan CORNET
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
#include <windows.h>
#include <strsafe.h>
#include <string.h>
#include <stdio.h>
#include "GetWindowsVersion.h"
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
#define MSG_DETECT_XP_OR_MORE "Scilab requires Windows XP or more."
#define MSG_DETECT_UNKNOW "Scilab does not support this unknown version of Windows."
#define MSG_DETECT_SSE_OR_MORE "Scilab requires SSE Instructions."
#define MSG_WARNING "Warning"
#define MSG_LOAD_LIBRARIES "Wscilex.exe failed with error %d: %s"
#define MAIN_FUNCTION "Windows_Main"
#define SCILAB_LIBRARY "scilab_windows"
#define ARG_NW "-nw"
#define ARG_NWNI "-nwni"
#define ARG_NOGUI "-nogui"
#define LENGTH_BUFFER_SECURITY 64
/*--------------------------------------------------------------------------*/
typedef int (*MYPROC1) (HINSTANCE, HINSTANCE , LPSTR szCmdLine, int iCmdShow);
/*--------------------------------------------------------------------------*/
/* BUG 6934 */
/* http://bugzilla.scilab.org/show_bug.cgi?id=6934 */
/* http://msdn.microsoft.com/en-us/library/chh3fb0k(VS.80).aspx */
#ifdef __INTEL_COMPILER
#pragma optimize("g", off)
#endif
/*--------------------------------------------------------------------------*/
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    int iExitCode = 0;
    HINSTANCE hinstLib = NULL;
    BOOL fFreeResult = FALSE, fRunTimeLinkSuccess = FALSE;

    if (GetWindowsVersion() == OS_ERROR	)
    {
        MessageBox(NULL, TEXT(MSG_DETECT_UNKNOW), TEXT(MSG_WARNING), MB_ICONWARNING);
        return -1;
    }

    if (GetWindowsVersion() < OS_WIN32_WINDOWS_XP	)
    {
        MessageBox(NULL, TEXT(MSG_DETECT_XP_OR_MORE), TEXT(MSG_WARNING), MB_ICONWARNING);
        return -1;
    }

    /* http://msdn.microsoft.com/en-us/library/ms724482(VS.85).aspx */
    if (!IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
    {
        MessageBox(NULL, TEXT(MSG_DETECT_SSE_OR_MORE), TEXT(MSG_WARNING), MB_ICONWARNING);
        return -1;
    }

    hinstLib = LoadLibrary(TEXT(SCILAB_LIBRARY));
    if (hinstLib != NULL)
    {
        MYPROC1 Windows_Main = NULL;

        /* launch main */
        Windows_Main = (MYPROC1) GetProcAddress(hinstLib, MAIN_FUNCTION);
        if (NULL != Windows_Main)
        {

#ifndef _DEBUG
            /* catch system errors msgbox (release mode only) */
            /* http://msdn.microsoft.com/en-us/library/ms680621(VS.85).aspx */
            UINT LastErrorMode = SetErrorMode( SEM_FAILCRITICALERRORS | SEM_NOALIGNMENTFAULTEXCEPT | SEM_NOGPFAULTERRORBOX );
            _try
            {
#endif
                fRunTimeLinkSuccess = TRUE;
                /* launch main */
                iExitCode = (Windows_Main)(hInstance, hPrevInstance, szCmdLine, iCmdShow);

#ifndef _DEBUG
            }
            _except (EXCEPTION_EXECUTE_HANDLER)
            {
            }
#endif
        }
        fFreeResult = FreeLibrary(hinstLib);
    }

    if (!fRunTimeLinkSuccess)
    {
        LPVOID lpMsgBuf;
        LPVOID lpDisplayBuf;

        DWORD dw = GetLastError();

        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpMsgBuf,
            0, NULL );

        lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
                                          (lstrlen((LPCTSTR)lpMsgBuf) + LENGTH_BUFFER_SECURITY) * sizeof(TCHAR));
        StringCchPrintf((LPTSTR)lpDisplayBuf,
                        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
                        TEXT(MSG_LOAD_LIBRARIES),
                        dw, lpMsgBuf);

        MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT(MSG_WARNING), MB_ICONERROR);

        LocalFree(lpMsgBuf);
        LocalFree(lpDisplayBuf);
        exit(1);
    }
    return iExitCode;
}
/*--------------------------------------------------------------------------*/
