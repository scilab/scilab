/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) DIGITEO - 2010 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
SCILAB_WINDOWS_IMPEXP int spawncommand(char *command, BOOL DetachProcess);

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
SCILAB_WINDOWS_IMPEXP BOOL DetectDetachProcessInCommandLine(char *command);

/**
* CreateOuput
* @param[in] pipe
* @param[in] DetachProcess
* @return output
*/
SCILAB_WINDOWS_IMPEXP char **CreateOuput(pipeinfo *pipe,BOOL DetachProcess);

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

#endif /* __SPAWNCOMMAND_H__ */
/*--------------------------------------------------------------------------*/
