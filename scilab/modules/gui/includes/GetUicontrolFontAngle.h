/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __GET_UICONTROL_FONTANGLE_H__
#define __GET_UICONTROL_FONTANGLE_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the font angle property of a uicontrol
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @return true if the font angle property has been correcty got
 */
int GetUicontrolFontAngle(sciPointObj * pobj);

#endif /* __GET_UICONTROL_FONTANGLE_H__ */
