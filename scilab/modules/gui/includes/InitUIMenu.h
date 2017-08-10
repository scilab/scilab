/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
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

#ifndef __INIT_UIMENU_H__
#define __INIT_UIMENU_H__

#include <stdlib.h>
#include "dynlib_gui.h"
#include "returnProperty.h"
#include "BOOL.h"

/**
 * Set the parent of a Menu
 *
 * @param sciObj the menu object
 * @param stackPointer adress in stack for the parent
 * @param valueType type of the parent in stack
 * @param nbRow number of row of the parent
 * @param nbCol number of columns of the parent
 * @return execution status
 */
GUI_IMPEXP int setMenuParent(int iObj, void* pvData, int valueType, int nbRow, int nbCol);

/**
 * Disable/Enable a menu attached to a graphics window or Scilab console
 *
 * @param pParentId the parent UID
 * @param name name of the menu
 * @param status is TRUE to enable the menu, false else
 */
GUI_IMPEXP void EnableMenu(int iParentId, char *name, BOOL status);

/**
 * Disable/Enable a submenu attached to a graphics window or Scilab console
 *
 * @param pParentId the figure UID
 * @param name name of the menu
 * @param position the position of the menu in its parent
 * @param status is TRUE to enable the menu, false else
 */
GUI_IMPEXP void EnableSubMenu(int iParentId, char *name, int position, BOOL status);

/**
 * Delete a menu attached to a graphics window or Scilab console
 *
 * @param pParentId the figure UID
 * @param name name of the menu
 */
GUI_IMPEXP void DeleteMenuWithName(int iParentId, char *name);

#endif /* !__INIT_UIMENU_H__ */
