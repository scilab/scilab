#ifndef __CALLJXGETMOUSE_H__
#define __CALLJXGETMOUSE_H__

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
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

#include "dynlib_gui.h"

GUI_IMPEXP void CallJxgetmouse(void);

GUI_IMPEXP void CallJxgetmouseWithOptions(int withMotion, int withRelease);

GUI_IMPEXP int getJxgetmouseMouseButtonNumber(void);

GUI_IMPEXP int getJxgetmouseWindowsID(void);

GUI_IMPEXP double getJxgetmouseXCoordinate(void);

GUI_IMPEXP double getJxgetmouseYCoordinate(void);
#endif

