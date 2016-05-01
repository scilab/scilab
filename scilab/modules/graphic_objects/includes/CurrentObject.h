/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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
