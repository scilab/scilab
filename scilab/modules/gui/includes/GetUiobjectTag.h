/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __GET_UIOBJECT_TAG_H__
#define __GET_UIOBJECT_TAG_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the tag property of a uicontrol or uimenu
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @return true if the tag property has been correcty got
 */
int GetUiobjectTag(sciPointObj * pobj);

#endif /* __GET_UIOBJECT_TAG_H__ */
