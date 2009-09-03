/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __DESTROY_UICONTROL_H__
#define __DESTROY_UICONTROL_H__

#include "dynlib_gui.h"
#include "ObjectStructure.h" /* sciPointObj */


/**
 * DestroyUicontrol
 * This function deletes Uicontrol structure.
 * @param sciPointObj * pthis pointer to the object structure
 * @return execution status
 */
GUI_IMPEXP int DestroyUicontrol (sciPointObj * pthis);

#endif

