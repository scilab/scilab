/* Copyright INRIA 2008 */
/* Vincent COUVERT */

#ifndef __LISTBOX_H__
#define __LISTBOX_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Create a new ListBox in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object
 */
void createListBox(sciPointObj* sciObj);

/**
 * Set the current figure as parent for a ListBox in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object   
 * @return true parent setting has be done without error
 */
int setCurentFigureAsListBoxParent(sciPointObj* sciObj);

#endif /* __LISTBOX_H__ */
