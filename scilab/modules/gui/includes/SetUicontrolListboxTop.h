/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __SET_UICONTROL_LISTBOXTOP_H__
#define __SET_UICONTROL_LISTBOXTOP_H__

#include "ObjectStructure.h"

/**
 * Set the listbox top property of a uicontrol (listbox)
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @param stackPointer position on the stack of the listbox top value to set
 * @param valueType type of the value on stack
 * @param nbRow number of rows of the value on stack
 * @param nbCol number of columns of the value on stack
 * @return true if the listbox top property set has been correcty done
 */
int SetUicontrolListboxTop(sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol);

#endif /* __SET_UICONTROL_LISTBOXTOP_H__ */
