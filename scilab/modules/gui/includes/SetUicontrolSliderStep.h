/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __SET_UICONTROL_SLIDERSTEP_H__
#define __SET_UICONTROL_SLIDERSTEP_H__

#include "ObjectStructure.h"

/**
 * Set the slider step property of a uicontrol (slider)
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @param stackPointer position on the stack of the slider step to set
 * @param valueType type of the value on stack
 * @param nbRow number of rows of the value on stack
 * @param nbCol number of columns of the value on stack
 * @return true if the slider step property set has been correcty done
 */
int SetUicontrolSliderStep(sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol);

#endif /* __SET_UICONTROL_SLIDERSTEP_H__ */
