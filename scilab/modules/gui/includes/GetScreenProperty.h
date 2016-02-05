/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
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
GUI_IMPEXP void* GetScreenProperty(void* _pvCtx, char *propertyName);

#endif /* !__GETSCREENPROPERTY_H__ */
