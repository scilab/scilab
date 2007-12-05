/* Copyright INRIA */
/* Vincent COUVERT 2007 */

#include "CallJxgetmouse.hxx"

using namespace org_scilab_modules_gui_events;

void CallJxgetmouse()
{
  Jxgetmouse::xgetmouse(getScilabJavaVM());
}

void CallJxgetmouseWithOptions(int withMotion, int withRelease)
{
  Jxgetmouse::xgetmouse(getScilabJavaVM(), withMotion, withRelease);
}

int getJxgetmouseMouseButtonNumber()
{
  return Jxgetmouse::getMouseButtonNumber(getScilabJavaVM());
}

double getJxgetmouseXCoordinate()
{
  return Jxgetmouse::getXCoordinate(getScilabJavaVM());
}

double getJxgetmouseYCoordinate()
{
  return Jxgetmouse::getYCoordinate(getScilabJavaVM());
}
