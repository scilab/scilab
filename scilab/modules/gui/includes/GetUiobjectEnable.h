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

#ifndef __GETUIOBJECTENABLE_H__
#define __GETUIOBJECTENABLE_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the enable status of a uicontrol or uimenu
 *
 * @param pobj Scilab object corresponding to the uicontrol or uimenu
 * @return true if the status is got without error
 */
int GetUiobjectEnable(sciPointObj * pobj);

#endif /* !__GETUIOBJECTENABLE_H__ */
