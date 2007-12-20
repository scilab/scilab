/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __SET_UIMENU_LABEL_H__
#define __SET_UIMENU_LABEL_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Set the label of an uimenu
 *
 * @param pobj Scilab object corresponding to the uimenu
 * @param stackPointer position on the stack of the label to set
 * @param valueType type of the value on stack
 * @param nbRow number of rows of the value on stack
 * @param nbCol number of columns of the value on stack
 * @return true is the set has been correcty done
 */
int SetUimenuLabel(sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol ) ;

#endif
