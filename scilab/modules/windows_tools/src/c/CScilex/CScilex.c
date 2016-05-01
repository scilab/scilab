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
#define MSG_LOAD_LIBRARIES "scilex.exe failed with error %d: %s"
#define MAIN_FUNCTION "Console_Main"
#define SCILAB_LIBRARY  "scilab_windows"
#define ARG_NW "-nw"
#define ARG_NWNI "-nwni"
#define ARG_NOGUI "-nogui"
#define LENGTH_BUFFER_SECURITY 64
/*--------------------------------------------------------------------------*/
typedef int (*MYPROC1) (int , char **);
/*--------------------------------------------------------------------------*/
/* BUG 6934 */
/* http://bugzilla.scilab.org/show_bug.cgi?id=6934 */
/* http://msdn.microsoft.com/en-us/library/chh3fb0k(VS.80).aspx */
#ifdef __INTEL_COMPILER
#pragma optimize("g", off)
#endif
/*--------------------------------------------------------------------------*/
int main (int argc, char **argv)
{
#define MAXCMDTOKENS 128
    int iExitCode = 0;
    UINT LastErrorMode = 0;
    HINSTANCE hinstLib = NULL;

    BOOL fFreeResult = FALSE, fRunTimeLinkSuccess = FALSE;

    int argcbis = -1;
    LPSTR argvbis[MAXCMDTOKENS];
    int i = 0;
    int FindNW = 0;

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

    for (i = 0; i < argc; i++)
    {
        if (_stricmp(argv[i], ARG_NW) == 0)
        {
            FindNW = 1;
        }
        if (_stricmp(argv[i], ARG_NWNI) == 0 )
        {
            FindNW = 1;
        }
        if (_stricmp(argv[i], ARG_NOGUI) == 0 )
        {
            FindNW = 1;
        }
    }

    if ( FindNW == 0 )
    {
        /* -nw added as first argument and not last */
        char *nwparam = NULL;
        nwparam = (char*)MALLOC((strlen(ARG_NW) + 1) * sizeof(char));
        strcpy_s(nwparam, (strlen(ARG_NW) + 1), ARG_NW);

        argvbis[0] = argv[0];
        argvbis[1] = nwparam;

        for (i = 1; i < argc; i++)
        {
            argvbis[i + 1] = argv[i];
        }
        argcbis = argc + 1;
    }
    else
    {
        for (i = 0; i < argc; i++)
        {
            argvbis[i] = argv[i];
        }
        argcbis = argc;
    }

    /* Disable system errors msgbox */
    LastErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);

    hinstLib = LoadLibrary(TEXT(SCILAB_LIBRARY));

    /* re enable system errors msgbox */
    SetErrorMode(LastErrorMode);

    if (hinstLib != NULL)
    {
        MYPROC1 Console_Main = NULL;

        /* launch main */
        Console_Main = (MYPROC1) GetProcAddress(hinstLib, MAIN_FUNCTION);

        if (NULL != Console_Main)
        {
            fRunTimeLinkSuccess = TRUE;

#ifndef _DEBUG
            /* catch system errors msgbox (release mode only) */
            /* http://msdn.microsoft.com/en-us/library/ms680621(VS.85).aspx */
            LastErrorMode = SetErrorMode( SEM_FAILCRITICALERRORS | SEM_NOALIGNMENTFAULTEXCEPT | SEM_NOGPFAULTERRORBOX );
            _try
            {
#endif
                iExitCode = (Console_Main)(argcbis, argvbis);

#ifndef _DEBUG
            }
            _except (EXCEPTION_EXECUTE_HANDLER)
            {
            }
#endif
        }
        fFreeResult = FreeLibrary(hinstLib);
    }

    if (! fRunTimeLinkSuccess)
    {
#define BUFFER_SIZE 512
        char buffer[BUFFER_SIZE];
        char *OutputMsg = NULL;
        DWORD dw = GetLastError();

        if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
                          dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                          buffer, BUFFER_SIZE, NULL) == 0)
        {
            StringCchPrintf(buffer, strlen("Unknown Error") + 1, "Unknown Error");
        }

        fprintf(stderr, "scilex can't launch scilab.\nError code : %lu\n", dw);
        OutputMsg = (char*)MALLOC((strlen(buffer) + 1) * sizeof(char));
        if (OutputMsg)
        {
            CharToOem(buffer, OutputMsg);
            fprintf(stderr, "%s\n", OutputMsg);
            FREE(OutputMsg);
        }
        exit(1);
    }
    return iExitCode;
}
/*--------------------------------------------------------------------------*/
