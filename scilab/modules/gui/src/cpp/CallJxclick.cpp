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


#include "CallJxclick.hxx"

using namespace org_scilab_modules_gui_events;

void CallJxclick()
{
  Jxclick::xclick(getScilabJavaVM());
}

int getJxclickMouseButtonNumber()
{
  return Jxclick::getMouseButtonNumber(getScilabJavaVM());
}

double getJxclickXCoordinate()
{
  return Jxclick::getXCoordinate(getScilabJavaVM());
}

double getJxclickYCoordinate()
{
  return Jxclick::getYCoordinate(getScilabJavaVM());
}

int getJxclickWindowID()
{
  return Jxclick::getWindowID(getScilabJavaVM());
}

char * getJxclickMenuCallback()
{
  return Jxclick::getMenuCallback(getScilabJavaVM());
}

void deleteMenuCallBack(char * menuCallBack)
{
  delete[] menuCallBack;
}
