/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GETUICONTROLPOSITION_H__
#define __GETUICONTROLPOSITION_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the position of a uicontrol
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @return the position (x, y, width, height)
 */
int GetUicontrolPosition(sciPointObj * pobj);

#endif
