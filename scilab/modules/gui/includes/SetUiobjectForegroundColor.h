/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __SET_UIOBJECT_FOREGROUNDCOLOR_H__
#define __SET_UIOBJECT_FOREGROUNDCOLOR_H__

#include "ObjectStructure.h"

/**
 * Set the foreground color of a uicontrol or an uimenu
 *
 * @param pobj Scilab object corresponding to the uicontrol or uimenu
 * @param stackPointer position on the stack of the color to set
 * @param valueType type of the value on stack
 * @param nbRow number of rows of the value on stack
 * @param nbCol number of columns of the value on stack
 * @return true is the set has been correcty done
 */
int SetUiobjectForegroundColor(sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol);

#endif /* __SET_UIOBJECT_FOREGROUNDCOLOR_H__ */
