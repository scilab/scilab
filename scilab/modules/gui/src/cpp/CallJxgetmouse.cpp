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
