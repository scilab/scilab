/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __GETLABELPROPERTY_H__
#define __GETLABELPROPERTY_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the label of a uicontrol or uimenu
 *
 * @param pobj Scilab object corresponding to the uicontrol or uimenu
 * @return the label
 */
int GetLabelProperty(sciPointObj * pobj);

#endif
