/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Return the style name corresponding to the integer value from UicontrolStyle enum
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "UnitsConversion.hxx"

using namespace org_scilab_modules_gui_bridge;

double ConvertFromPoint(int value, int newUnit, sciPointObj *sciObj)
{
  long int *position = NULL;

  int height = 0;

  /* Get the component height from java */
  if( (sciObj != NULL) && (sciGetEntityType(sciObj) == SCI_FIGURE) ) /* Uicontrol figure */
    {
      height = sciGetHeight(sciObj);
      newUnit = PIXELS_UNITS;
    }
  else if ( (sciObj != NULL) && (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) ) /* Frame style uicontrol */
    {
      position = CallScilabBridge::getFramePosition(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex);
      height = position[3];
    }
  else if ( sciObj != NULL ) /* All other uicontrol styles */
    {
      position = CallScilabBridge::getWidgetPosition(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex);
      height = position[3];
    }
  
  switch(newUnit)
    {
    case POINTS_UNITS:
      return (double) value;
    case NORMALIZED_UNITS:
      return ((double) value) / (height * POINT_PER_INCH / CallScilabBridge::getScreenResolution(getScilabJavaVM()));
    case INCHES_UNITS:
      return ((double) value) / POINT_PER_INCH;
    case CENTIMETERS_UNITS:
      return ((double) value) / POINT_PER_INCH * CM_PER_INCH;
    case PIXELS_UNITS:
      return ((double) value) / POINT_PER_INCH * CallScilabBridge::getScreenResolution(getScilabJavaVM());
    default:
      sciprint(_("FontUnits property value must be a single string: points, normalized, inches, centimeters or pixels.\n"));
      return 0.0;
    }
}

int ConvertToPoint(double value, int oldUnit, sciPointObj *sciObj)
{
  long int *position = NULL;

  int height = 0;

  /* Get the component height from java */
  if( (sciObj != NULL) && (sciGetEntityType(sciObj) == SCI_FIGURE) ) /* Uicontrol figure */
    {
      height = sciGetHeight(sciObj);
      oldUnit = PIXELS_UNITS;
    }
  else if ( (sciObj != NULL) && (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) ) /* Frame style uicontrol */
    {
      position = CallScilabBridge::getFramePosition(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex);
      height = position[3];
    }
  else if ( sciObj != NULL )  /* All other uicontrol styles */
    {
      position = CallScilabBridge::getWidgetPosition(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex);
      height = position[3];
    }

  switch(oldUnit)
    {
    case POINTS_UNITS:
      return (int) value;
    case NORMALIZED_UNITS:
      return (int) (value * height * POINT_PER_INCH / CallScilabBridge::getScreenResolution(getScilabJavaVM()));
    case INCHES_UNITS:
      return (int) (value * POINT_PER_INCH);
    case CENTIMETERS_UNITS:
      return (int) (value * POINT_PER_INCH / CM_PER_INCH);
    case PIXELS_UNITS:
      return (int) (value * POINT_PER_INCH / CallScilabBridge::getScreenResolution(getScilabJavaVM()));
    default:
      sciprint(_("FontUnits property value must be a single string: points, normalized, inches, centimeters or pixels.\n"));
      return 0;
    }
}

double ConvertFromPixel(int value, int newUnit, sciPointObj *sciObj)
{
  if( (sciObj != NULL) && (sciGetEntityType(sciObj) == SCI_FIGURE) ) /* Uicontrol figure */
    {
      newUnit = PIXELS_UNITS;
    }
  return ConvertFromPoint(value / CallScilabBridge::getScreenResolution(getScilabJavaVM()) * POINT_PER_INCH, newUnit, sciObj);
}

int ConvertToPixel(double value, int oldUnit, sciPointObj *sciObj)
{
  if( (sciObj != NULL) && (sciGetEntityType(sciObj) == SCI_FIGURE) ) /* Uicontrol figure */
    {
      oldUnit = PIXELS_UNITS;
    }
  return ConvertToPoint(value, oldUnit, sciObj) * CallScilabBridge::getScreenResolution(getScilabJavaVM()) / POINT_PER_INCH;
}

