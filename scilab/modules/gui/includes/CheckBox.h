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

#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Create a new CheckBox in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object
 */
void createCheckBox(sciPointObj* sciObj);

/**
 * Set the current figure as parent for a CheckBox in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object   
 * @return true parent setting has be done without error
 */
int setCurentFigureAsCheckBoxParent(sciPointObj* sciObj);

#endif /* __CHECKBOX_H__ */
