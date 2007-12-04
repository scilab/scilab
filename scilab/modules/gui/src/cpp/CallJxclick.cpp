/* Copyright INRIA */
/* Vincent COUVERT 2007 */

#include "CallJxclick.hxx"

using namespace org_scilab_modules_gui_events;

void CallJxclick()
{
  Jxclick::xclick(getScilabJavaVM());
}

int getMouseButtonNumber()
{
  return Jxclick::getMouseButtonNumber(getScilabJavaVM());
}

double getXCoordinate()
{
  return Jxclick::getXCoordinate(getScilabJavaVM());
}

double getYCoordinate()
{
  return Jxclick::getYCoordinate(getScilabJavaVM());
}

int getWindowID()
{
  return Jxclick::getWindowID(getScilabJavaVM());
}

char * getMenuCallback()
{
  return Jxclick::getMenuCallback(getScilabJavaVM());
}
