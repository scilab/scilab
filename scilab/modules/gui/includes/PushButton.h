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

#ifndef __PUSHBUTTON_H__
#define __PUSHBUTTON_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Create a new PushButton in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object
 */
void createPushButton(sciPointObj* sciObj);

/**
 * Set the current figure as parent for a Push Button in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object   
 * @return true parent setting has be done without error
 */
int setCurentFigureAsPushButtonParent(sciPointObj* sciObj);

#endif /* __PUSHBUTTON_H__ */
