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

#ifndef __SET_UIOBJECT_TAG_H__
#define __SET_UIOBJECT_TAG_H__
#include "dynlib_gui.h"
#include "ObjectStructure.h"

/**
 * Set the tag property of a uicontrol, a uimenu or a figure
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @param stackPointer position on the stack of the tag value to set
 * @param valueType type of the value on stack
 * @param nbRow number of rows of the value on stack
 * @param nbCol number of columns of the value on stack
 * @return true if the tag property set has been correcty done
 */
GUI_IMPEXP int SetUiobjectTag(sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol);

/**
 * Destroy the tag for a uicontrol a uimenu or a figure handle.
 * It must be done in C++ code since it must use delete[].
 */
GUI_IMPEXP void destroyUiobjectTag(sciPointObj * pobj);

#endif /* __SET_UIOBJECT_TAG_H__ */
