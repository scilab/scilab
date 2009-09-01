/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
SCILAB_WINDOWS_IMPEXP BOOL SendCommandToAnotherScilab(char *ScilabWindowNameSource,char *ScilabWindowNameDestination,char *CommandLine);

/**
* Get a command from another scilab
* @param[in] Title
* @param[in] command to send
* @return TRUE or FALSE
*/
SCILAB_WINDOWS_IMPEXP BOOL GetCommandFromAnotherScilab(char *TitleWindowSend,char *CommandLine);

/**
* Receive data from another scilab
* @param[in] Handle on Window
* @param[in] COPYDATASTRUCT
* @return TRUE or FALSE
*/
SCILAB_WINDOWS_IMPEXP BOOL ReceiveFromAnotherScilab(HWND hWndSend,PCOPYDATASTRUCT MyCopyDataStruct); 

#endif /*__WMCOPYDATA_H__ */
/*--------------------------------------------------------------------------*/ 
