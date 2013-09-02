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
#ifndef __SCILAB_MAIN__H__
#define __SCILAB_MAIN__H__

#include "dynlib_scilab_windows.h"
#include "realmain.h" /* InitScriptType */

/**
* common main for windows
* @param[in] no startup
* @param[in] path script
* @param[in] path type
* @param[in] size path
* @param[in] stacksize
*/
SCILAB_WINDOWS_IMPEXP int sci_windows_main ( int *nos, char *path, InitScriptType pathtype, int *lpath, int memory);

#endif /* __SCILAB_MAIN__H__ */
/*--------------------------------------------------------------------------*/
