/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __GET_UICONTROL_MAX_H__
#define __GET_UICONTROL_MAX_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the max property of a uicontrol
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @return true if the max property has been correcty got
 */
int GetUicontrolMax(sciPointObj * pobj);

#endif /* __GET_UICONTROL_MAX_H__ */
