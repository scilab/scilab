/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __GET_UIOBJECT_FOREGROUNDCOLOR_H__
#define __GET_UIOBJECT_FOREGROUNDCOLOR_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the foreground color of a uicontrol
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @return true if the color has been correcty got
 */
int GetUiobjectForegroundColor(sciPointObj * pobj);

#endif /* __GET_UIOBJECT_FOREGROUNDCOLOR_H__ */
