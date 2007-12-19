/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Sets the foreground of an uicontrol or an uimenu */

#include "SetUiobjectForegroundColor.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUiobjectForegroundColor(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  /* Color can be [R, G, B] or "R|G|B" */

  int redInt = 0, greenInt = 0, blueInt = 0, nbvalues = 0;

  double * allcolors = NULL;
  
  float redFloat = 0.0, greenFloat = 0.0, blueFloat = 0.0;
  
  if (valueType == sci_strings)
    {
      if(nbCol != 1) /* More than one string */
        {
          sciprint(_("%s property value must be \"R|G|B\" or [R, G, B].\n"), "ForegroundColor");
          return SET_PROPERTY_ERROR;
        }
      
      nbvalues = sscanf(getStringFromStack(stackPointer), "%e|%e|%e", &redFloat, &greenFloat, &blueFloat);

      if (nbvalues != 3) /* Wrong format string */
        {
          sciprint(_("%s property value must be \"R|G|B\" or [R, G, B].\n"), "ForegroundColor");
          return SET_PROPERTY_ERROR;
        }

      redInt = (int) (redFloat * 255);
      greenInt = (int) (greenFloat * 255);
      blueInt = (int) (blueFloat * 255);
      
    }
  else if (valueType == sci_matrix)
    {
      if(nbCol != 3 || nbRow != 1) /* Wrong matrix size */
        {
          sciprint(_("%s property value must be \"R|G|B\" or [R, G, B].\n"), "ForegroundColor");
          return SET_PROPERTY_ERROR;
        }

       allcolors = getDoubleMatrixFromStack(stackPointer);
       redInt = (int) (allcolors[0] * 255);
       greenInt = (int) (allcolors[1] * 255);
       blueInt = (int) (allcolors[2] * 255);
     
    }
  else
    {
      /* Wrong value type */
      sciprint(_("%s property value must be \"R|G|B\" or [R, G, B].\n"), "ForegroundColor");
      return SET_PROPERTY_ERROR;
    }

 
  if(sciGetEntityType(sciObj) == SCI_UIMENU)  /* Sets the foreground of an uimenu */
    {
          CallScilabBridge::setMenuForegroundColor(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, redInt, greenInt, blueInt);
          return SET_PROPERTY_SUCCEED;
    }
  else if(sciGetEntityType(sciObj) == SCI_UICONTROL)  /* Sets the foreground of an uicontrol */
    {
      if (strcmp(pUICONTROL_FEATURE(sciObj)->style, "pushbutton")==0) 
        {
          CallScilabBridge::setPushButtonForegroundColor(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, redInt, greenInt, blueInt);
          return SET_PROPERTY_SUCCEED;
        }
      else
        {
          /* Unimplemented uicontrol type */
          sciprint(_("No %s property for uicontrols of style: %s.\n"), "ForegroundColor", pUICONTROL_FEATURE(sciObj)->style);
          return SET_PROPERTY_ERROR;
        }
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "ForegroundColor");
      return SET_PROPERTY_ERROR;
    }
}

