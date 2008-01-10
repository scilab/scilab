/* Copyright INRIA 2008 */
/* Vincent COUVERT */

#ifndef __POPUPMENU_H__
#define __POPUPMENU_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Create a new PopupMenu in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object
 */
void createPopupMenu(sciPointObj* sciObj);

/**
 * Set the current figure as parent for a PopupMenu in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object   
 * @return true parent setting has be done without error
 */
int setCurentFigureAsPopupMenuParent(sciPointObj* sciObj);

#endif /* __POPUPMENU_H__ */
