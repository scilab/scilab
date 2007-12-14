/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __SETUICONTROLPARENT_H__
#define __SETUICONTROLPARENT_H__

#include "ObjectStructure.h"

/**
 * Set the parent of a uicontrol
 *
 * @param pobj Scilab object corresponding to the uicontrol or uimenu
 * @param stackPointer position on the stack of the label to set
 * @param valueType type of the value on stack
 * @param nbRow number of rows of the value on stack
 * @param nbCol number of columns of the value on stack
 * @return true is the set has been correcty done
 */
int SetUicontrolParent(sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol);

#endif
