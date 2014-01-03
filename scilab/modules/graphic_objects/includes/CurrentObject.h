/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __CURRENT_OBJECT_H__
#define __CURRENT_OBJECT_H__

#include "dynlib_graphic_objects.h"
#include "BOOL.h"
/**
 * Returns UID of the current selected object.
 */
GRAPHIC_OBJECTS_IMPEXP int getCurrentObject(void);

/**
 * Modify the current object if needed.
 */
GRAPHIC_OBJECTS_IMPEXP void setCurrentObject(int iUID);

/**
 * To know if an object is the selected one.
 */
GRAPHIC_OBJECTS_IMPEXP BOOL isCurrentObject(int iUID);

/**
 * get parent object
 */
GRAPHIC_OBJECTS_IMPEXP int getParentObject(int iUID);
GRAPHIC_OBJECTS_IMPEXP void setParentObject(int iUID, int iParent);

#endif /* !__CURRENT_OBJECT_H__ */
