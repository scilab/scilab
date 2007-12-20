/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __GET_UICONTROL_MIN_H__
#define __GET_UICONTROL_MIN_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the min property of a uicontrol
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @return true if the min property has been correcty got
 */
int GetUicontrolMin(sciPointObj * pobj);

#endif /* __GET_UICONTROL_MIN_H__ */
