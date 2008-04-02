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
  long int *position = 0;

  /* Get the component height from java */
  if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrol */
    {
      position = CallScilabBridge::getFramePosition(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex);
    }
  else /* All other uicontrol styles */
    {
      position = CallScilabBridge::getWidgetPosition(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex);
    }
  
  switch(newUnit)
    {
    case POINTS_UNITS:
      return (double) value;
    case NORMALIZED_UNITS:
      return ((double) value) / (position[3] * POINT_PER_INCH / CallScilabBridge::getScreenResolution(getScilabJavaVM()));
    case INCHES_UNITS:
      return ((double) value) / POINT_PER_INCH;
    case CENTIMETERS_UNITS:
      return ((double) value) / POINT_PER_INCH / CM_PER_INCH;
    case PIXELS_UNITS:
      return ((double) value) / POINT_PER_INCH * CallScilabBridge::getScreenResolution(getScilabJavaVM());
    default:
      sciprint(_("FontUnits property value must be a single string: points, normalized, inches, centimeters or pixels.\n"));
      return 0.0;
    }
}

int ConvertToPoint(double value, int oldUnit, sciPointObj *sciObj)
{
  long int *position = 0;

  /* Get the component height from java */
  if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrol */
    {
      position = CallScilabBridge::getFramePosition(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex);
    }
  else /* All other uicontrol styles */
    {
      position = CallScilabBridge::getWidgetPosition(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex);
    }

  switch(oldUnit)
    {
    case POINTS_UNITS:
      return (int) value;
    case NORMALIZED_UNITS:
      return (int) (value * position[3] * POINT_PER_INCH / CallScilabBridge::getScreenResolution(getScilabJavaVM()));
    case INCHES_UNITS:
      return (int) (value * POINT_PER_INCH);
    case CENTIMETERS_UNITS:
      return (int) (value * POINT_PER_INCH * CM_PER_INCH);
    case PIXELS_UNITS:
      return (int) (value * POINT_PER_INCH / CallScilabBridge::getScreenResolution(getScilabJavaVM()));
    default:
      sciprint(_("FontUnits property value must be a single string: points, normalized, inches, centimeters or pixels.\n"));
      return 0;
    }
}
