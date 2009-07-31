/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SETUIMENUCHECKED_H__
#define __SETUIMENUCHECKED_H__

#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Set the checked status of an uimenu
 *
 * @param pobj Scilab object corresponding to the uimenu
 * @param stackPointer position on the stack of the status to set
 * @param valueType type of the value on stack
 * @param nbRow number of rows of the value on stack
 * @param nbCol number of columns of the value on stack
 * @return true if the set has been correcty done
 */
int SetUimenuChecked(sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;

#endif /* !__SETUIMENUCHECKED_H__ */
