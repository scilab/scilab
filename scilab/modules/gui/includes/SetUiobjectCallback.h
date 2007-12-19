/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __SETUIOBJECTCALLBACK_H__
#define __SETUIOBJECTCALLBACK_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Set the callback string of a uicontrol or uimenu
 *
 * @param pobj Scilab object corresponding to the uicontrol or uimenu
 * @param stackPointer position on the stack of the label to set
 * @param valueType type of the value on stack
 * @param nbRow number of rows of the value on stack
 * @param nbCol number of columns of the value on stack
 * @return true is the set has been correcty done
 */
int SetUiobjectCallback(sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol ) ;

#endif
