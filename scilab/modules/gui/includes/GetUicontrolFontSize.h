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

#ifndef __GET_UICONTROL_FONTSIZE_H__
#define __GET_UICONTROL_FONTSIZE_H__
#include "dynlib_gui.h"
#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Get the size of a uicontrol font
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @return true if the size property has been correcty got
 */
GUI_IMPEXP int GetUicontrolFontSize(sciPointObj * pobj);

#endif /* __GET_UICONTROL_FONTSIZE_H__ */
