/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __GETUICONTROLPOSITION_H__
#define __GETUICONTROLPOSITION_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the position of a uicontrol
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @return the position (x, y, width, height)
 */
int GetUicontrolPosition(sciPointObj * pobj);

#endif
