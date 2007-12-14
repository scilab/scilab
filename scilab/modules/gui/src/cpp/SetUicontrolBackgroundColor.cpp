/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Sets the background of an uicontrol object */

#include "SetUicontrolBackgroundColor.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolBackgroundColor(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  // Color can be [R, G, B]
  // or "R|G|B"

  int redInt = 0, greenInt = 0, blueInt = 0, nbvalues = 0;

  double * allcolors = NULL;
  
  float redDouble = 0.0, greenDouble = 0.0, blueDouble = 0.0;
  
  if (valueType == sci_strings)
    {
      if(nbCol != 1)
        {
          // Parent must be a single string
          return SET_PROPERTY_ERROR;
        }
      
      nbvalues = sscanf(getStringFromStack(stackPointer), "%e|%e|%e", &redDouble, &greenDouble, &blueDouble);

      if (nbvalues != 3)
        {
          // Could not read color values
          return SET_PROPERTY_ERROR;
        }

      redInt = (int) (redDouble * 255);
      greenInt = (int) (greenDouble * 255);
      blueInt = (int) (blueDouble * 255);
      
    }
  else if (valueType == sci_matrix)
    {
       if(nbCol != 3 || nbRow != 1)
        {
          // Parent must be a [R, G, B]
          return SET_PROPERTY_ERROR;
        }

       allcolors = getDoubleMatrixFromStack(stackPointer);
       redInt = (int) (allcolors[0] * 255);
       greenInt = (int) (allcolors[1] * 255);
       blueInt = (int) (allcolors[2] * 255);
     
    }
  else
    {
      // Do not know how to set the parent
      Scierror(999, _("Wrong value for uicontrol BackgroundColor property, must be [R, G, B] or \"R|G|B\".\n"));
      return SET_PROPERTY_ERROR;
    }

  if (strcmp(pUICONTROL_FEATURE(sciObj)->style, "pushbutton")==0)
    {
      CallScilabBridge::setPushButtonBackgroundColor(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, redInt, greenInt, blueInt);
      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      Scierror(999, _("Do not known how to set BackgroundColor for uicontrols of style: %s.\n"), pUICONTROL_FEATURE(sciObj)->style);
      return SET_PROPERTY_ERROR;
    }
}

