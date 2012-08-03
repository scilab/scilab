/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __CURRENT_OBJECT_H__
#define __CURRENT_OBJECT_H__

#include "dynlib_graphic_objects.h"
/**
 * Returns UID of the current selected object.
 */
GRAPHIC_OBJECTS_IMPEXP char const* getCurrentObject(void);

/**
 * Modify the current object if needed.
 */
GRAPHIC_OBJECTS_IMPEXP void setCurrentObject(char const* UID);

/**
 * To know if an object is the selected one.
 */
GRAPHIC_OBJECTS_IMPEXP BOOL isCurrentObject(char const* UID);

#endif /* !__CURRENT_OBJECT_H__ */
