/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SETUIOBJECTENABLE_H__
#define __SETUIOBJECTENABLE_H__

#include <stddef.h>

#include "dynlib_gui.h"
#include "returnProperty.h"

/**
 * Set the enable status of a uicontrol or uimenu
 *
 * @param pobjUID Scilab object corresponding to the uicontrol or uimenu
 * @param stackPointer position on the stack of the status to set
 * @param valueType type of the value on stack
 * @param nbRow number of rows of the value on stack
 * @param nbCol number of columns of the value on stack
 * @return true if the set has been correcty done
 */
GUI_IMPEXP int SetUiobjectEnable(void* _pvCtx, char *pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;

#endif /* !__SETUIOBJECTENABLE_H__ */
