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
#ifndef __GW_WINDOWS_TOOLS_H__
#define __GW_WINDOWS_TOOLS_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_windows_tools.h"
/*--------------------------------------------------------------------------*/
WINDOWS_TOOLS_IMPEXP int gw_windows_tools(void);
//Scilab 6
int sci_dos(char *fname, void* pvApiCtx);
/*--------------------------------------------------------------------------*/
#endif /* __GW_WINDOWS_TOOLS_H__ */


