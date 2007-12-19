/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Sets the background of an uicontrol object */

#include "SetUicontrolBackgroundColor.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolBackgroundColor(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  /* Color can be [R, G, B] or "R|G|B" */

  int redInt = 0, greenInt = 0, blueInt = 0, nbvalues = 0;

  double * allcolors = NULL;
  
  float redFloat = 0.0, greenFloat = 0.0, blueFloat = 0.0;
  
  if (valueType == sci_strings)
    {
      if(nbCol != 1)
        {
          /* Wrong string size */
          sciprint(_("BackgroundColor property value must be \"R|G|B\" or [R, G, B].\n"));
          return SET_PROPERTY_ERROR;
        }
      
      nbvalues = sscanf(getStringFromStack(stackPointer), "%e|%e|%e", &redFloat, &greenFloat, &blueFloat);

      if (nbvalues != 3)
        {
          /* Wrong string format */
          sciprint(_("BackgroundColor property value must be \"R|G|B\" or [R, G, B].\n"));
          return SET_PROPERTY_ERROR;
        }

      redInt = (int) (redFloat * 255);
      greenInt = (int) (greenFloat * 255);
      blueInt = (int) (blueFloat * 255);
      
    }
  else if (valueType == sci_matrix)
    {
       if(nbCol != 3 || nbRow != 1)
        {
          /* Wrong matrix size */
          sciprint(_("BackgroundColor property value must be \"R|G|B\" or [R, G, B].\n"));
          return SET_PROPERTY_ERROR;
        }

       allcolors = getDoubleMatrixFromStack(stackPointer);
       redInt = (int) (allcolors[0] * 255);
       greenInt = (int) (allcolors[1] * 255);
       blueInt = (int) (allcolors[2] * 255);
     
    }
  else
    {
      /* Wrong datatype */
      sciprint(_("BackgroundColor property value must be \"R|G|B\" or [R, G, B].\n"));
      return SET_PROPERTY_ERROR;
    }

  if (strcmp(pUICONTROL_FEATURE(sciObj)->style, "pushbutton")==0)
    {
      CallScilabBridge::setPushButtonBackgroundColor(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, redInt, greenInt, blueInt);
      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      /* Unimplmented uicontrol style */
      sciprint(_("No BackgroundColor for uicontrols of style: %s.\n"), pUICONTROL_FEATURE(sciObj)->style);
      return SET_PROPERTY_ERROR;
    }
}

