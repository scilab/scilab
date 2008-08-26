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

#ifndef __SET_UICONTROL_STRING_H__
#define __SET_UICONTROL_STRING_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Set the string property of an uicontrol
 *
 * @param pobj Scilab object corresponding to the uicontrol
 * @param stackPointer position on the stack of the string to set
 * @param valueType type of the value on stack
 * @param nbRow number of rows of the value on stack
 * @param nbCol number of columns of the value on stack
 * @return true is the set has been correcty done
 */
int SetUicontrolString(sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;

#endif /* __SET_UICONTROL_STRING_H__ */
