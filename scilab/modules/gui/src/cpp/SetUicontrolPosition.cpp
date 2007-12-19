/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Sets the position of an uicontrol object */

#include "SetUicontrolPosition.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolPosition(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  // Position can be [x, y, width, height]
  // or "x|y|width|height"

  int xInt = 0, yInt = 0, widthInt = 0, heightInt, nbvalues = 0;

  double * allValues = NULL;
  
  float xFloat = 0.0, yFloat = 0.0, widthFloat = 0.0, heightFloat = 0.0;
  
  if (valueType == sci_strings)
    {
      if(nbCol != 1)
        {
          // Parent must be a single string
          return SET_PROPERTY_ERROR;
        }
      
      nbvalues = sscanf(getStringFromStack(stackPointer), "%e|%e|%e|%e", &xFloat, &xFloat, &widthFloat, &heightFloat);

      if (nbvalues != 4)
        {
          // Could not read all position values
          return SET_PROPERTY_ERROR;
        }

      xInt = (int) (xFloat * 255);
      yInt = (int) (yFloat * 255);
      widthInt = (int) (widthFloat * 255);
      heightInt = (int) (heightFloat * 255);
      
    }
  else if (valueType == sci_matrix)
    {
       if(nbCol != 4 || nbRow != 1)
        {
          // Parent must be a [x, y, width, height]
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
      // Do not know how to set the parent
      Scierror(999, _("Wrong value for uicontrol Position property, must be [x, y, width, height] or \"x|y|width|height\".\n"));
      return SET_PROPERTY_ERROR;
    }

  if (strcmp(pUICONTROL_FEATURE(sciObj)->style, "pushbutton")==0)
    {

      CallScilabBridge::setPushButtonPosition(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, xInt, sciGetHeight(sciGetParent(sciObj)) - yInt - heightInt, widthInt, heightInt);

      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      Scierror(999, _("Do not known how to set Position for uicontrols of style: %s.\n"), pUICONTROL_FEATURE(sciObj)->style);
      return SET_PROPERTY_ERROR;
    }
}

