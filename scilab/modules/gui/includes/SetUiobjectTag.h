/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __SET_UIOBJECT_TAG_H__
#define __SET_UIOBJECT_TAG_H__

#include "ObjectStructure.h"

/**
 * Set the tag property of a uicontrol or a uimenu
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @param stackPointer position on the stack of the tag value to set
 * @param valueType type of the value on stack
 * @param nbRow number of rows of the value on stack
 * @param nbCol number of columns of the value on stack
 * @return true if the tag property set has been correcty done
 */
int SetUiobjectTag(sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol);

#endif /* __SET_UIOBJECT_TAG_H__ */
