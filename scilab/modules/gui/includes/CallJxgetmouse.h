#ifndef __CALLJXGETMOUSE_H__
#define __CALLJXGETMOUSE_H__

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "dynlib_gui.h"

GUI_IMPEXP void CallJxgetmouse(void);

GUI_IMPEXP void CallJxgetmouseWithOptions(int withMotion, int withRelease);

GUI_IMPEXP int getJxgetmouseMouseButtonNumber(void);

GUI_IMPEXP int getJxgetmouseWindowsID(void);

GUI_IMPEXP double getJxgetmouseXCoordinate(void);

GUI_IMPEXP double getJxgetmouseYCoordinate(void);
#endif

