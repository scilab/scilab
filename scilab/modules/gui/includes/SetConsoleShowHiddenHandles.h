/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __SETCONSOLESHOWHIDDENHANDLES_H__
#define __SETCONSOLESHOWHIDDENHANDLES_H__

#include <stdlib.h>

#include "dynlib_gui.h"

/**
 * Set ShowHiddenHandles property of the console
 *
 * @param pobj Scilab object corresponding to the console
 * @param stackPointer position on the stack of the status to set
 * @param valueType type of the value on stack
 * @param nbRow number of rows of the value on stack
 * @param nbCol number of columns of the value on stack
 * @return true if the set has been correcty done
 */
GUI_IMPEXP int SetConsoleShowHiddenHandles(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);

/**
 * Set ShowHiddenProperties property of the console
 *
 * @param pobj Scilab object corresponding to the console
 * @param stackPointer position on the stack of the status to set
 * @param valueType type of the value on stack
 * @param nbRow number of rows of the value on stack
 * @param nbCol number of columns of the value on stack
 * @return true if the set has been correcty done
 */
GUI_IMPEXP int SetConsoleShowHiddenProperties(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);

/**
 * Set UseDeprecatedSkin property of the console
 *
 * @param pobj Scilab object corresponding to the console
 * @param stackPointer position on the stack of the status to set
 * @param valueType type of the value on stack
 * @param nbRow number of rows of the value on stack
 * @param nbCol number of columns of the value on stack
 * @return true if the set has been correcty done
 */
GUI_IMPEXP int SetConsoleUseDeprecatedLF(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);

#endif /* !__SETCONSOLESHOWHIDDENHANDLES_H__ */
