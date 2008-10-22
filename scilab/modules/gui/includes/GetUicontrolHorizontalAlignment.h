/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
