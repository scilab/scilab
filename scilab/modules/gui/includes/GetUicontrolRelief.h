/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __GET_UICONTROL_RELIEF_H__
#define __GET_UICONTROL_RELIEF_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the relief property of a uicontrol
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @return true if the relief property has been correcty got
 */
int GetUicontrolRelief(sciPointObj * pobj);

#endif /* __GET_UICONTROL_RELIEF_H__ */
