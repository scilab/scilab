/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GETCONSOLESHOWHIDDENHANDLES_H__
#define __GETCONSOLESHOWHIDDENHANDLES_H__

#include <stdlib.h>

#include "dynlib_gui.h"

/**
 * Get ShowHiddenHandles property of the console
 *
 * @param pobj Scilab object corresponding to the console
 * @return true if the get has been correcty done
 */
GUI_IMPEXP int GetConsoleShowHiddenHandles(void* _pvCtx, char *pobjUID);

#endif /* !__GETCONSOLESHOWHIDDENHANDLES_H__ */
