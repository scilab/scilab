/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __WITHTK_H__
#define __WITHTK_H__

#include "machine.h" /* C2F */
#include "dynlib_tclsci.h"
#include "BOOL.h" /* BOOL*/

/** 
* check if it is with TCL/TK
* @param rep 1 (WITH) or 0 (WITHOUT)
* @return 0
*/
TCLSCI_IMPEXP int C2F(withtk)(int *rep);

/** 
* check if it is with TCL/TK
* @return TRUE or FALSE
*/
TCLSCI_IMPEXP BOOL withtk(void);

#endif /* _WITHTK_H__ */
/*--------------------------------------------------------------------------*/ 
