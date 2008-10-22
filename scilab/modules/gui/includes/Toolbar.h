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

#include "ObjectStructure.h"
#include "returnProperty.h"
#include "BOOL.h"

/**
 * Set the visibility of the toolbar
 *
 * @param figNum number of the parent figure (-1 for Console window)
 * @param status TRUE to set the toolbar visible
 */
void setToolbarVisible(int figNum, BOOL status);

/**
 * Get the visibility of the toolbar
 *
 * @param figNum number of the parent figure (-1 for Console window)
 * @return TRUE if the toolbar is visible
 */
BOOL isToolbarVisible(int figNum);

#endif /* __TOOLBAR_H__ */
