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
#ifndef __WINDOWS_MAIN_H__
#define __WINDOWS_MAIN_H__

#include <Windows.h>
#include "dynlib_scilab_windows.h"

/**
* Main for scilab with GUI
* @param [in] current instance of the application.
* @param [in] previous instance of the application
* @param [in] command line for the application
* @param [in] Specifies how the window is to be shown
* @return 0
*/
SCILAB_WINDOWS_IMPEXP int Windows_Main (HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR szCmdLine, int iCmdShow);

#endif /* __WINDOWS_MAIN_H__ */
/*--------------------------------------------------------------------------*/ 
