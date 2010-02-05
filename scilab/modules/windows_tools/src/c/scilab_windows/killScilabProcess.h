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
#ifndef __KILLSCILABPROCESS_H__
#define __KILLSCILABPROCESS_H__

#include "dynlib_scilab_windows.h"

/**
* Kill current scilab process
* @param[in] exit code value
*/
SCILAB_WINDOWS_IMPEXP void killScilabProcess(int exitCode);

#endif /* __KILLSCILABPROCESS_H__ */ 
/*--------------------------------------------------------------------------*/
