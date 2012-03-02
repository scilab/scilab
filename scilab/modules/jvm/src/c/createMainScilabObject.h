
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
#ifndef __CREATEMAINSCILABOBJECT_H__
#define __CREATEMAINSCILABOBJECT_H__

#include "BOOL.h" /* BOOL */

/**
* Create Scilab object (scilab main java)
* @return TRUE or FALSE
*/
BOOL createMainScilabObject(void);

BOOL finishMainScilabObject(void);

BOOL executeInitialHooks(void);

BOOL canCloseMainScilabObject(void);

void forceCloseMainScilabObject(void);

#endif /* __CREATEMAINSCILABOBJECT_H__ */
/*--------------------------------------------------------------------------*/
