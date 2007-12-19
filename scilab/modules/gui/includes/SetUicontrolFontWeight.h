/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __SET_UICONTROL_FONTWEIGHT_H__
#define __SET_UICONTROL_FONTWEIGHT_H__

#include "ObjectStructure.h"

/**
 * Set the font weight color of a uicontrol
 *
 * @param pobj Scilab object corresponding to the uicontrol or uimenu
 * @param stackPointer position on the stack of the font weight to set
 * @param valueType type of the value on stack
 * @param nbRow number of rows of the value on stack
 * @param nbCol number of columns of the value on stack
 * @return true is the set has been correcty done
 */
int SetUicontrolFontWeight(sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol);

#endif /* __SET_UICONTROL_FONTWEIGHT_H__ */
