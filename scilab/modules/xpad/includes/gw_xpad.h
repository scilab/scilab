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

#ifndef __GW_XPAD__
#define __GW_XPAD__
/*--------------------------------------------------------------------------*/
#include "dynlib_xpad.h"
/*--------------------------------------------------------------------------*/
XPAD_IMPEXP int gw_xpad(void);
XPAD_IMPEXP int sci_xpad(char* fname, unsigned long l);
XPAD_IMPEXP int sci_xpad_highlightline(char* fname, unsigned long l);
/*--------------------------------------------------------------------------*/
#endif /* __GW_XPAD__ */
/*--------------------------------------------------------------------------*/
