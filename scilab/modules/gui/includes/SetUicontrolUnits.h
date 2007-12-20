/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __SET_UICONTROL_UNITS_H__
#define __SET_UICONTROL_UNITS_H__

#include "ObjectStructure.h"

/**
 * Set the units of a uicontrol
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @param stackPointer position on the stack of the units value to set
 * @param valueType type of the value on stack
 * @param nbRow number of rows of the value on stack
 * @param nbCol number of columns of the value on stack
 * @return true if the units property set has been correcty done
 */
int SetUicontrolUnits(sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol);

#endif /* __SET_UICONTROL_UNITS_H__ */
