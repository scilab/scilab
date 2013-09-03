/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/

#ifndef __CONSOLE_MAIN_H__
#define __CONSOLE_MAIN_H__

#include "dynlib_scilab_windows.h"

/**
* Main for scilab with no gui
* @param [in] argc : number of arguments
* @param [in] argv : values of arguments
* @return 0
*/
SCILAB_WINDOWS_IMPEXP int Console_Main(int argc, char **argv);

#endif /* __CONSOLE_MAIN_H__ */