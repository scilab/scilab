/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __GET_UICONTROL_BACKGROUNDCOLOR_H__
#define __GET_UICONTROL_BACKGROUNDCOLOR_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the background color of a uicontrol
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @return true if the color has been correcty got
 */
int GetUicontrolBackgroundColor(sciPointObj * pobj);

#endif /* __GET_UICONTROL_BACKGROUNDCOLOR_H__ */
