/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GETSCREENPROPERTY_H__
#define __GETSCREENPROPERTY_H__
#include "dynlib_gui.h"
/*
 * Get a screen (root) property
 *
 * @param stack address where the property name is stored
 * @param propertyName the name of the property to get
 * @return execution status
 */
GUI_IMPEXP int GetScreenProperty(void* _pvCtx, char *propertyName);

#endif /* !__GETSCREENPROPERTY_H__ */
