/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __GET_UICONTROL_VERTICALALIGNMENT_H__
#define __GET_UICONTROL_VERTICALALIGNMENT_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the vertical alignment of the text in a uicontrol
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @return true if the vertical alignment property has been correcty got
 */
int GetUicontrolVerticalAlignment(sciPointObj * pobj);

#endif /* __GET_UICONTROL_VERTICALALIGNMENT_H__ */
