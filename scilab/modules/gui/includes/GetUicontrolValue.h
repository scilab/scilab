/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __GET_UICONTROL_VALUE_H__
#define __GET_UICONTROL_VALUE_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the value property of a uicontrol
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @return true if the value property has been correcty got
 */
int GetUicontrolValue(sciPointObj * pobj);

#endif /* __GET_UICONTROL_VALUE_H__ */
