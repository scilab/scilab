/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __GW_XCOS__
#define __GW_XCOS__
/*--------------------------------------------------------------------------*/
#include "dynlib_xcos.h"
/*--------------------------------------------------------------------------*/
XCOS_IMPEXP int gw_xcos(void);
XCOS_IMPEXP int sci_Xcos(char* fname, unsigned long l);
XCOS_IMPEXP int sci_xcosNotify(char* fname, unsigned long l);
/*--------------------------------------------------------------------------*/
#endif /* __GW_XCOS__ */
/*--------------------------------------------------------------------------*/
