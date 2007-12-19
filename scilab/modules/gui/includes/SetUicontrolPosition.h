/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __SETUICONTROLPOSITION_H__
#define __SETUICONTROLPOSITION_H__

#include "ObjectStructure.h"

/**
 * Set the position of a uicontrol
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @param stackPointer position on the stack of the position to set
 * @param valueType type of the value on stack
 * @param nbRow number of rows of the value on stack
 * @param nbCol number of columns of the value on stack
 * @return true is the set has been correcty done
 */
int SetUicontrolPosition(sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol);

#endif
