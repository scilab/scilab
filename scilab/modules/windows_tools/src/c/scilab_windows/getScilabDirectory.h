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
#ifndef __GETSCILABDIRECTORY_H__
#define __GETSCILABDIRECTORY_H__

#include "dynlib_scilab_windows.h"
#include "BOOL.h" /* BOOL */

/**
* get scilab directory (windows)
* @param [in]
* @return
*/
SCILAB_WINDOWS_IMPEXP char *getScilabDirectory(BOOL UnixStyle);

#endif /* __GETSCILABDIRECTORY_H__ */
/*--------------------------------------------------------------------------*/
