/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __EDITBOX_H__
#define __EDITBOX_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Create a new Edit Box in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object
 */
void createEditBox(sciPointObj* sciObj);

/**
 * Set the current figure as parent for a Edit Box in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object   
 * @return true parent setting has be done without error
 */
int setCurentFigureAsEditBoxParent(sciPointObj* sciObj);

#endif /* __EDITBOX_H__ */
