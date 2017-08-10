/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) DIGITEO - 2010 - Allan CORNET
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
#ifndef __SPAWNCOMMAND_H__
#define __SPAWNCOMMAND_H__
/*--------------------------------------------------------------------------*/
#include <Windows.h>
#include "dynlib_scilab_windows.h"
#include "BOOL.h" /* BOOL */
/*--------------------------------------------------------------------------*/
typedef struct pipeinfo
{
    HANDLE pipe;
    unsigned char *OutputBuffer;
    int NumberOfLines;
} pipeinfo;

#ifndef SCILAB_WINDOWS_EXPORTS
pipeinfo SCILAB_WINDOWS_IMPEXP pipeSpawnOut;
pipeinfo SCILAB_WINDOWS_IMPEXP pipeSpawnErr;
#endif
/*--------------------------------------------------------------------------*/
/**
* spawn a command
* @param[in] command
* @param[in] DetachProcess (if we want detach to scilab process)
* @return 0
*/
SCILAB_WINDOWS_IMPEXP int spawncommand(wchar_t *command, BOOL DetachProcess);

/**
* ReadFromPipe (in or out)
* @param[in] pipe handle
* @return 0
*/
SCILAB_WINDOWS_IMPEXP DWORD WINAPI ReadFromPipe (LPVOID args);

/**
* check if we have a '&' (detach process)
* @param[in] command
* @return TRUE or FALSE
*/
SCILAB_WINDOWS_IMPEXP BOOL DetectDetachProcessInCommandLine(wchar_t *command);

/**
* CreateOuput
* @param[in] pipe
* @param[in] DetachProcess
* @return output
*/
SCILAB_WINDOWS_IMPEXP char **CreateOuput(pipeinfo *pipe, BOOL DetachProcess);

/**
* Close pipe
* @param[in] pipe
* @return 0
*/
SCILAB_WINDOWS_IMPEXP int ClosePipeInfo (pipeinfo pipe);

/**
* Call cmd.exe windows shell
* @param[in] command to execute
* @param[out] exit code returned by cmd
*/
SCILAB_WINDOWS_IMPEXP int CallWindowsShell(char *command);

/**
* Call cmd.exe windows shell
* @param[in] command to execute
* @param[out] exit code returned by cmd
*/
SCILAB_WINDOWS_IMPEXP int CallWindowsShellW(wchar_t* _pstCommand);

#endif /* __SPAWNCOMMAND_H__ */
/*--------------------------------------------------------------------------*/
