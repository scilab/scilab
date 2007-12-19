/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __GETUIOBJECTCALLBACK_H__
#define __GETUIOBJECTCALLBACK_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the callback string of a uicontrol or uimenu
 *
 * @param pobj Scilab object corresponding to the uicontrol or uimenu
 * @return true if the callback is got without error
 */
int GetUiobjectCallback(sciPointObj * pobj);

#endif
