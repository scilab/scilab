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

#ifndef __TOOLBAR_H__
#define __TOOLBAR_H__
#include "dynlib_gui.h"
#include "returnProperty.h"
#include "BOOL.h"

/**
 * Set the visibility of the toolbar
 *
 * @param pParentUID the parent UID (figure or console)
 * @param status TRUE to set the toolbar visible
 */
GUI_IMPEXP void setToolbarVisible(char *pParentUID, BOOL status);

/**
 * Get the visibility of the toolbar
 *
 * @param pParentUID the parent UID (figure or console)
 * @return TRUE if the toolbar is visible
 */
GUI_IMPEXP BOOL isToolbarVisible(char *pParentUID);

#endif /* __TOOLBAR_H__ */
