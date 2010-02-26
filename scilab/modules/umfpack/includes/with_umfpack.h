/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRu
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __WITH_UMFPACK_H__
#define __WITH_UMFPACK_H__

#include "machine.h" /* C2F */
#include "dynlib_umfpack.h"
#include "BOOL.h"

/** 
* check if it is with UMFPACK
* @param rep 1 (WITH) or 0 (WITHOUT)
* @return 0
*/
UMFPACK_IMPEXP int C2F(withumfpack)(int *rep);

/** 
* check if it is with UMFPACK
* @return TRUE or FALSE
*/
UMFPACK_IMPEXP BOOL withumfpack(void);

#endif /* __WITH_UMFPACK_H__ */
/*--------------------------------------------------------------------------*/ 
