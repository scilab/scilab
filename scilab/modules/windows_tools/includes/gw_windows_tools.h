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


