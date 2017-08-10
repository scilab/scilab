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


#include "CallJxgetmouse.hxx"

using namespace org_scilab_modules_gui_events;

void CallJxgetmouse()
{
    Jxgetmouse::xgetmouse(getScilabJavaVM());
}

void CallJxgetmouseWithOptions(int withMotion, int withRelease)
{
    Jxgetmouse::xgetmouse(getScilabJavaVM(), BOOLtobool((BOOL)withMotion), BOOLtobool((BOOL)withRelease));
}

int getJxgetmouseMouseButtonNumber()
{
    return Jxgetmouse::getMouseButtonNumber(getScilabJavaVM());
}

int getJxgetmouseWindowsID()
{
    return Jxgetmouse::getWindowsID(getScilabJavaVM());
}

double getJxgetmouseXCoordinate()
{
    return Jxgetmouse::getXCoordinate(getScilabJavaVM());
}

double getJxgetmouseYCoordinate()
{
    return Jxgetmouse::getYCoordinate(getScilabJavaVM());
}
