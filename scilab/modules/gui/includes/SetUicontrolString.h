/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __SET_UICONTROL_STRING_H__
#define __SET_UICONTROL_STRING_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Set the string property of an uicontrol
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @param stackPointer position on the stack of the string to set
 * @param valueType type of the value on stack
 * @param nbRow number of rows of the value on stack
 * @param nbCol number of columns of the value on stack
 * @return true is the set has been correcty done
 */
int SetUicontrolString(sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol ) ;

#endif /* __SET_UICONTROL_STRING_H__ */
