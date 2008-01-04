/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#ifndef __GET_UICONTROL_FONTNAME_H__
#define __GET_UICONTROL_FONTNAME_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the font name property of a uicontrol
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @return true if the font name property has been correcty got
 */
int GetUicontrolFontName(sciPointObj * pobj);

#endif /* __GET_UICONTROL_FONTNAME_H__ */
