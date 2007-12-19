/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __GET_UICONTROL_FONTUNITS_H__
#define __GET_UICONTROL_FONTUNITS_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the units of a uicontrol font
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @return true if the units property has been correcty got
 */
int GetUicontrolFontUnits(sciPointObj * pobj);

#endif /* __GET_UICONTROL_FONTUNITS_H__ */
