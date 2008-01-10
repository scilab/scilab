/* Copyright INRIA 2008 */
/* Vincent COUVERT */

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
