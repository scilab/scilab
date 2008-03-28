/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Sets the position of an uicontrol object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolPosition.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolPosition(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  // Position can be [x, y, width, height]
  // or "x|y|width|height"

  int xInt = 0, yInt = 0, widthInt = 0, heightInt, nbvalues = 0;

  double * allValues = NULL;
  
  float xDouble = 0.0, yDouble = 0.0, widthDouble = 0.0, heightDouble = 0.0;
  
  if (valueType == sci_strings)
    {
      if(nbCol != 1)
        {
          sciprint(_("Position property value must be \"x|y|width|height\" or [x, y, width, height].\n"));
          return SET_PROPERTY_ERROR;
        }
      
      nbvalues = sscanf(getStringFromStack(stackPointer), "%e|%e|%e|%e", &xDouble, &yDouble, &widthDouble, &heightDouble);

      if (nbvalues != 4)
        {
          sciprint(_("Position property value must be \"x|y|width|height\" or [x, y, width, height].\n"));
          return SET_PROPERTY_ERROR;
        }

      xInt = (int) (xDouble);
      yInt = (int) (yDouble);
      widthInt = (int) (widthDouble);
      heightInt = (int) (heightDouble);
      
    }
  else if (valueType == sci_matrix)
    {
       if(nbCol != 4 || nbRow != 1)
        {
          sciprint(_("Position property value must be \"x|y|width|height\" or [x, y, width, height].\n"));
          return SET_PROPERTY_ERROR;
        }

       allValues = getDoubleMatrixFromStack(stackPointer);
       xInt = (int) (allValues[0]);
       yInt = (int) (allValues[1]);
       widthInt = (int) (allValues[2]);
       heightInt = (int) (allValues[3]);
     
    }
  else
    {
      sciprint(_("Position property value must be \"x|y|width|height\" or [x, y, width, height].\n"));
      return SET_PROPERTY_ERROR;
    }

  if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrols */
    {
      CallScilabBridge::setFramePosition(getScilabJavaVM(), 
                                          pUICONTROL_FEATURE(sciObj)->hashMapIndex, 
                                          xInt, 
                                          sciGetHeight(sciGetParent(sciObj)) - yInt - heightInt, 
                                          widthInt, 
                                          heightInt);
    }
  else if( sciGetEntityType(sciObj) == SCI_FIGURE ) /* Uicontrol figure */
  {
    return (int)(sciInitScreenPosition(sciObj, xInt, yInt) & sciSetWindowDim(sciObj, widthInt, heightInt));
  }
  else /* All other uicontrol styles */
    {
      CallScilabBridge::setWidgetPosition(getScilabJavaVM(), 
                                          pUICONTROL_FEATURE(sciObj)->hashMapIndex, 
                                          xInt, 
                                          sciGetHeight(sciGetParent(sciObj)) - yInt - heightInt, 
                                          widthInt, 
                                          heightInt);
      
      /* Special case for Sliders: set orientation */
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_SLIDER)
        {
          if (widthInt > heightInt) /* Horizontal ScrollBar */
            {
              CallScilabBridge::setSliderHorizontal(getScilabJavaVM(), 
                                                          pUICONTROL_FEATURE(sciObj)->hashMapIndex);
            }
          else /* Vertical ScrollBar */
            {
              CallScilabBridge::setSliderVertical(getScilabJavaVM(), 
                                                          pUICONTROL_FEATURE(sciObj)->hashMapIndex);
            }
        }
    }
  return SET_PROPERTY_SUCCEED;
}

