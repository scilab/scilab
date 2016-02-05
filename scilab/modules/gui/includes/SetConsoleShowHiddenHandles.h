/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
