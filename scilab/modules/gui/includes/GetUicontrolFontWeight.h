/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __GET_UICONTROL_FONTWEIGHT_H__
#define __GET_UICONTROL_FONTWEIGHT_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the weight of a uicontrol font
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @return true if the font has been correcty got
 */
int GetUicontrolFontWeight(sciPointObj * pobj);

#endif /* __GET_UICONTROL_FONTWEIGHT_H__ */
