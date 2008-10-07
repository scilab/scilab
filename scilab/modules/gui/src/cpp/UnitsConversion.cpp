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

double ConvertFromPoint(int value, int newUnit, sciPointObj *sciObj, BOOL widthAsRef)
{
  int *position = NULL;

  int refSize = 0;

  sciPointObj *parent = NULL;

  /* Get the component height from java */
  if(sciGetEntityType(sciObj) == SCI_FIGURE) /* Figure */
    {
      if (widthAsRef == TRUE)
        {
          refSize = (int)CallScilabBridge::getScreenWidth(getScilabJavaVM());
        }
      else
        {
          refSize = (int)CallScilabBridge::getScreenHeight(getScilabJavaVM());
        }
      newUnit = PIXELS_UNITS;
    }
  else /* Uicontrol */
    {
      parent = sciGetParent(sciObj);
      if (parent == NULL && newUnit == NORMALIZED_UNITS) /* Parent not yet set */
        {
          return 0.0;
        }
    }

  switch(newUnit)
    {
    case POINTS_UNITS:
       return (double) value;
    case NORMALIZED_UNITS:
      if(sciGetEntityType(parent) == SCI_FIGURE) /* Figure */
        {
          if (widthAsRef == TRUE)
            {
              refSize = sciGetWidth(parent);
            }
          else
            {
              refSize = sciGetHeight(parent);
            }
        }
      else /* Frame */
        {
          position = CallScilabBridge::getFramePosition(getScilabJavaVM(),
                                                        pUICONTROL_FEATURE(parent)->hashMapIndex);
          
          if (widthAsRef == TRUE)
            {
              refSize = position[2];
            }
          else
            {
              refSize = position[3];
            }

          delete[] position;
        }
      return ((double) value) / (refSize * POINT_PER_INCH / CallScilabBridge::getScreenResolution(getScilabJavaVM()));
    case INCHES_UNITS:
       return ((double) value) / POINT_PER_INCH;
    case CENTIMETERS_UNITS:
       return ((double) value) * CM_PER_INCH / POINT_PER_INCH;
    case PIXELS_UNITS:
       return ((double) value) * CallScilabBridge::getScreenResolution(getScilabJavaVM()) / POINT_PER_INCH;
    default:
      sciprint(_("%s: Wrong value for '%s' property: points, normalized, inches, centimeters or pixels expected.\n"), "ConvertFromPoint", "FontUnits");
      return 0.0;
    }
}

int ConvertToPoint(double value, int oldUnit, sciPointObj *sciObj, BOOL widthAsRef)
{
  int *position = NULL;

  int refSize = 0;

  sciPointObj *parent = NULL;

  /* Get the component height from java */
  if(sciGetEntityType(sciObj) == SCI_FIGURE) /* Figure */
    {
      /* The parent is the screen */
      if (widthAsRef == TRUE)
        {
          refSize = (int)CallScilabBridge::getScreenWidth(getScilabJavaVM());
        }
      else
        {
          refSize = (int)CallScilabBridge::getScreenHeight(getScilabJavaVM());
        }
      oldUnit = PIXELS_UNITS;
    }
  else /* Uicontrol */
    {
      parent = sciGetParent(sciObj);
      if (parent == NULL && oldUnit == NORMALIZED_UNITS) /* Parent not yet set */
        {
          return 0;
        }
    }

  switch(oldUnit)
    {
    case POINTS_UNITS:
      return (int) value;
    case NORMALIZED_UNITS:
      if(sciGetEntityType(parent) == SCI_FIGURE) /* Figure */
        {
          if (widthAsRef == TRUE)
            {
              refSize = sciGetWidth(parent);
            }
          else
            {
              refSize = sciGetHeight(parent);
            }
        }
      else /* Frame */
        {
          position = CallScilabBridge::getFramePosition(getScilabJavaVM(),
                                                        pUICONTROL_FEATURE(parent)->hashMapIndex);
          
          if (widthAsRef == TRUE)
            {
              refSize = position[2];
            }
          else
            {
              refSize = position[3];
            }

          delete[] position;
        }
      return (int) (value * refSize * POINT_PER_INCH / CallScilabBridge::getScreenResolution(getScilabJavaVM()));
    case INCHES_UNITS:
      return (int) (value * POINT_PER_INCH);
    case CENTIMETERS_UNITS:
      return (int) (value * POINT_PER_INCH / CM_PER_INCH);
    case PIXELS_UNITS:
      return (int) (value * POINT_PER_INCH / CallScilabBridge::getScreenResolution(getScilabJavaVM()));
    default:
      sciprint(_("%s: Wrong value for '%s' property: points, normalized, inches, centimeters or pixels expected.\n"), "ConvertToPoint", "FontUnits");
      return 0;
    }
}

double ConvertFromPixel(int value, int newUnit, sciPointObj *sciObj, BOOL widthAsRef)
{
  if( (sciObj != NULL) && (sciGetEntityType(sciObj) == SCI_FIGURE) ) /* Uicontrol figure */
    {
      newUnit = PIXELS_UNITS;
    }

  if (newUnit == PIXELS_UNITS) 
    {
      return (double) value;
    }
  else
    {
      return ConvertFromPoint(ConvertToPoint(value, PIXELS_UNITS, sciObj, widthAsRef), newUnit, sciObj, widthAsRef);
    }
}

int ConvertToPixel(double value, int oldUnit, sciPointObj *sciObj, BOOL widthAsRef)
{
  if( (sciObj != NULL) && (sciGetEntityType(sciObj) == SCI_FIGURE) ) /* Uicontrol figure */
    {
      oldUnit = PIXELS_UNITS;
    }

  if (oldUnit==PIXELS_UNITS)
    {
      return (int) value;
    }
  else
    {
      return (int) ConvertFromPoint(ConvertToPoint(value, oldUnit, sciObj, widthAsRef), PIXELS_UNITS, sciObj, widthAsRef);
    }
}

