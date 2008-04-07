/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Get the position of an uicontrol 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GETSCREENPROPERTY_H__
#define __GETSCREENPROPERTY_H__

/*
 * Get a screen (root) property
 * 
 * @param stack address where the property name is stored
 * @param valueType type of the property name
 * @param nbRow number of row of the property name
 * @param nbCol number of column of the property name
 * @return execution status
 */
int GetScreenProperty(int stackPointer, int valueType, int nbRow, int nbCol);

#endif /* !__GETSCREENPROPERTY_H__ */
