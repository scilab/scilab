/* Copyright INRIA 2008 */
/* Vincent COUVERT */

#ifndef __RADIOBUTTON_H__
#define __RADIOBUTTON_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Create a new RadioButton in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object
 */
void createRadioButton(sciPointObj* sciObj);

/**
 * Set the current figure as parent for a RadioButton in Scilab GUIs
 *
 * @param sciObj the corresponding Scilab object   
 * @return true parent setting has be done without error
 */
int setCurentFigureAsRadioButtonParent(sciPointObj* sciObj);

#endif /* __RADIOBUTTON_H__ */
