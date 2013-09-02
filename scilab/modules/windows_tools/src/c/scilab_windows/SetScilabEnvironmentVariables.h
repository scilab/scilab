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
#ifndef __SETSCILABENVIRONMENTVARIABLES_H__
#define __SETSCILABENVIRONMENTVARIABLES_H__

#include "dynlib_scilab_windows.h"

/**
* Set Some environment variables for Scilab (Windows)
* @param[in] default path of scilab
*/
SCILAB_WINDOWS_IMPEXP void SetScilabEnvironmentVariables(char *DefaultSCIPATH);

SCILAB_WINDOWS_IMPEXP void SciEnvForWindows(void);

#endif /* __SETSCILABENVIRONMENTVARIABLES_H__ */
/*--------------------------------------------------------------------------*/
