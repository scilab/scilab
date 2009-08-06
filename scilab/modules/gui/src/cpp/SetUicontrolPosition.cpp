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

int SetUicontrolPosition(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  // Position can be [x, y, width, height]
  // or "x|y|width|height"

  int xInt = 0, yInt = 0, widthInt = 0, heightInt, nbvalues = 0;

  double * allValues = NULL;
  
  float xDouble = 0.0, yDouble = 0.0, widthDouble = 0.0, heightDouble = 0.0;

  int * returnValues = NULL;

  sciPointObj *parent = NULL;

  int status = 0;
  
  if (stackPointer == -1) /* Default values setting */
    {
      xInt = 20;
      yInt = 40;
      widthInt = 40;
      heightInt = 20;
    }
  else
    {
      if (valueType == sci_strings)
        {
          if(nbCol != 1)
            {
              Scierror(999, _("Wrong size for '%s' property: A string or a 1 x %d real row vector expected.\n"), "Position", 4);
              return SET_PROPERTY_ERROR;
            }
          
          nbvalues = sscanf(getStringFromStack(stackPointer), "%e|%e|%e|%e", &xDouble, &yDouble, &widthDouble, &heightDouble);
          
          if (nbvalues != 4)
            {
              Scierror(999, _("Wrong value for '%s' property: A string or a 1 x %d real row vector expected.\n"), "Position", 4);
              return SET_PROPERTY_ERROR;
            }
          
          xInt = ConvertToPixel(xDouble, pUICONTROL_FEATURE(sciObj)->units, sciObj, TRUE);
          yInt = ConvertToPixel(yDouble, pUICONTROL_FEATURE(sciObj)->units, sciObj, FALSE);
          widthInt = ConvertToPixel(widthDouble, pUICONTROL_FEATURE(sciObj)->units, sciObj, TRUE);
          heightInt = ConvertToPixel(heightDouble, pUICONTROL_FEATURE(sciObj)->units, sciObj, FALSE);
          
        }
      else if (valueType == sci_matrix)
        {
          if(nbCol != 4 || nbRow != 1)
            {
              Scierror(999, _("Wrong size for '%s' property: A string or a 1 x %d real row vector expected.\n"), "Position", 4);
              return SET_PROPERTY_ERROR;
            }
          
          allValues = getDoubleMatrixFromStack(stackPointer);
          xInt = ConvertToPixel(allValues[0], pUICONTROL_FEATURE(sciObj)->units, sciObj, TRUE);
          yInt = ConvertToPixel(allValues[1], pUICONTROL_FEATURE(sciObj)->units, sciObj, FALSE);
          widthInt = ConvertToPixel(allValues[2], pUICONTROL_FEATURE(sciObj)->units, sciObj, TRUE);
          heightInt = ConvertToPixel(allValues[3], pUICONTROL_FEATURE(sciObj)->units, sciObj, FALSE);
          
        }
      else
        {
          Scierror(999, _("Wrong type for '%s' property: A string or a 1 x %d real row vector expected.\n"), "Position", 4);
          return SET_PROPERTY_ERROR;
        }
    }

  if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrols */
    {
      parent = sciGetParent(sciObj);
      if (parent != NULL && sciGetEntityType(parent)==SCI_UICONTROL)
        {
          /* Parent is a frame and position is relative to parent */
           returnValues = CallScilabBridge::getFramePosition(getScilabJavaVM(),
                                                             pUICONTROL_FEATURE(parent)->hashMapIndex);
           xInt = returnValues[0] + xInt;
           yInt = returnValues[1] + yInt;
        }

      CallScilabBridge::setFramePosition(getScilabJavaVM(), 
                                         pUICONTROL_FEATURE(sciObj)->hashMapIndex, 
                                         xInt, 
                                         yInt, 
                                         widthInt, 
                                         heightInt);
    }
  else if( sciGetEntityType(sciObj) == SCI_FIGURE ) /* Uicontrol figure */
  {
    /* disable protection since this function will call Java */
    disableFigureSynchronization(sciObj);
    status = sciSetDimension(sciObj, widthInt, heightInt) ;
    enableFigureSynchronization(sciObj);
    return (int)(sciInitScreenPosition(sciObj, xInt, yInt) & status);
    //return (int)(sciInitScreenPosition(sciObj, xInt, yInt) & sciSetWindowDim(sciObj, widthInt, heightInt));
  }
  else /* All other uicontrol styles */
    {

      parent = sciGetParent(sciObj);
      if (parent != NULL && sciGetEntityType(parent)==SCI_UICONTROL)
        {
          /* Parent is a frame and position is relative to parent */
           returnValues = CallScilabBridge::getFramePosition(getScilabJavaVM(),
                                                             pUICONTROL_FEATURE(parent)->hashMapIndex);
           xInt = returnValues[0] + xInt;
           yInt = returnValues[1] + yInt;
		   delete [] returnValues;
        }

      CallScilabBridge::setWidgetPosition(getScilabJavaVM(), 
                                          pUICONTROL_FEATURE(sciObj)->hashMapIndex, 
                                          xInt, 
                                          yInt, 
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

