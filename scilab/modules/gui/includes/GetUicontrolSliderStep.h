/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __GET_UICONTROL_SLIDERSTEP_H__
#define __GET_UICONTROL_SLIDERSTEP_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the slider step property of a uicontrol (slider)
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @return true if the slider step property has been correcty got
 */
int GetUicontrolSliderStep(sciPointObj * pobj);

#endif /* __GET_UICONTROL_SLIDERSTEP_H__ */
