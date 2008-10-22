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

#ifndef __CONTEXTMENU_H__
#define __CONTEXTMENU_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Create a new ContextMenu in Scilab GUIs
 *
 * @param menuLabels labels of the menu
 * @param nbLabels the number of labels
 * @return the label of the menu selected
 */
//char * createContextMenu(char **menuLabels, int nbLabels);

/**
 * Create a new empty ContextMenu in Scilab GUIs
 *
 * @param sciObj Scilab corresponding object
 */
void createContextMenu(sciPointObj* sciObj);

/**
 * Display the ContextMenu and waits for a user input
 *
 * @param sciObj Scilab corresponding object
 */
char* uiWaitContextMenu(sciPointObj* sciObj);

#endif /* __CONTEXTMENU_H__ */
