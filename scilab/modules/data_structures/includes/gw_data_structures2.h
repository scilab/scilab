/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GW_DATA_STRUCTURES2__
#define __GW_DATA_STRUCTURES2__
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <windows.h>
#include <stdio.h>
#endif
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
int gw_data_structures2(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_getfield)(char *fname,unsigned long fname_len);
int C2F(sci_setfield)(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_DATA_STRUCTURES2__ */
/*--------------------------------------------------------------------------*/

