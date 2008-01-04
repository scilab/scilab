/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __GET_UICONTROL_HORIZONTALALIGNMENT_H__
#define __GET_UICONTROL_HORIZONTALALIGNMENT_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the horizontal alignment of the text in a uicontrol
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @return true if the horizontal alignment property has been correcty got
 */
int GetUicontrolHorizontalAlignment(sciPointObj * pobj);

#endif /* __GET_UICONTROL_HORIZONTALALIGNMENT_H__ */
