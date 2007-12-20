/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __GET_UICONTROL_STRING_H__
#define __GET_UICONTROL_STRING_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the string property of an uicontrol
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @return the string
 */
int GetUicontrolString(sciPointObj * pobj);

#endif /* __GET_UICONTROL_STRING_H__ */
