/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __SET_UICONTROL_VERTICALALIGNMENT_H__
#define __SET_UICONTROL_VERTICALALIGNMENT_H__

#include "ObjectStructure.h"

/**
 * Set the vertical alignment property of a uicontrol (text...)
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @param stackPointer position on the stack of the vertical alignment to set
 * @param valueType type of the value on stack
 * @param nbRow number of rows of the value on stack
 * @param nbCol number of columns of the value on stack
 * @return true if the verticalalignment property set has been correcty done
 */
int SetUicontrolVerticalAlignment(sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol);

#endif /* __SET_UICONTROL_VERTICALALIGNMENT_H__ */
