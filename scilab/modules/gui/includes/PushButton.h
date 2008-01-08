/* Copyright INRIA 2007 */
/* Vincent COUVERT */

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
