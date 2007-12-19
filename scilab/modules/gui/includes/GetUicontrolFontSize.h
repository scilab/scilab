/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __GET_UICONTROL_FONTSIZE_H__
#define __GET_UICONTROL_FONTSIZE_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the size of a uicontrol font
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @return true if the size property has been correcty got
 */
int GetUicontrolFontSize(sciPointObj * pobj);

#endif /* __GET_UICONTROL_FONTSIZE_H__ */
