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

#ifndef __EDITBOX_H__
#define __EDITBOX_H__
#include "dynlib_gui.h"
#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Create a new Edit Box in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object
 */
GUI_IMPEXP void createEditBox(sciPointObj* sciObj);

/**
 * Set the current figure as parent for a Edit Box in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object   
 * @return true parent setting has be done without error
 */
GUI_IMPEXP int setCurentFigureAsEditBoxParent(sciPointObj* sciObj);

#endif /* __EDITBOX_H__ */
