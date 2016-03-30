/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
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
#ifndef __WMCOPYDATA_H__
#define __WMCOPYDATA_H__

#include <windows.h>
#include "dynlib_scilab_windows.h"
#include "PATH_MAX.h"
#include "BOOL.h"

typedef struct tagMYREC
{
    char  CommandFromAnotherScilab[PATH_MAX];
}
MYREC;

/**
* Send a command to another Scilab
* @param[in] Title Window source
* @param[in] Title Window destination
* @param[in] command to send
* @return TRUE or FALSE
*/
SCILAB_WINDOWS_IMPEXP BOOL SendCommandToAnotherScilab(char *ScilabWindowNameSource, char *ScilabWindowNameDestination, char *CommandLine);

/**
* Get a command from another scilab
* @param[in] Title
* @param[in] command to send
* @return TRUE or FALSE
*/
SCILAB_WINDOWS_IMPEXP BOOL GetCommandFromAnotherScilab(char *TitleWindowSend, char *CommandLine);

/**
* Receive data from another scilab
* @param[in] Handle on Window
* @param[in] COPYDATASTRUCT
* @return TRUE or FALSE
*/
SCILAB_WINDOWS_IMPEXP BOOL ReceiveFromAnotherScilab(HWND hWndSend, PCOPYDATASTRUCT MyCopyDataStruct);

#endif /*__WMCOPYDATA_H__ */
/*--------------------------------------------------------------------------*/
