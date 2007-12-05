/* Copyright INRIA */
/* Vincent COUVERT 2007 */

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
