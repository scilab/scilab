/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Sets the font weight of an uicontrol object */

#include "SetUicontrolFontWeight.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolFontWeight(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  /* Font weight can be light, normal, demi or bold */

  char * fontWeight = NULL; 

  if (valueType == sci_strings)
    {
      if(nbCol != 1)
        {
          /* Wrong string size */
          sciprint(_("FontWeight property value must be a single string: light, normal, demi or bold.\n"));
          return SET_PROPERTY_ERROR;
        }
      
      fontWeight = getStringFromStack(stackPointer);

      if (strcmp(fontWeight, "light") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->fontWeight = LIGHT_FONT;
        }
      else if(strcmp(fontWeight, "normal") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->fontWeight = NORMAL_FONT;
        }
      else if(strcmp(fontWeight, "demi") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->fontWeight = DEMI_FONT;
        }
      else if(strcmp(fontWeight, "bold") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->fontWeight = BOLD_FONT;
        }
      else
        {
          /* Wrong string format */
          sciprint(_("FontWeight property value must be a single string: light, normal, demi or bold.\n"));
          return SET_PROPERTY_ERROR;
        }
      
      if (strcmp(pUICONTROL_FEATURE(sciObj)->style, "pushbutton")==0)
        {
          CallScilabBridge::setPushButtonFontWeight(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, fontWeight);
          return SET_PROPERTY_SUCCEED;
        }
      else
        {
          /* Unimplmented uicontrol style */
          sciprint(_("No %s property for uicontrols of style: %s.\n"), "FontWeight", pUICONTROL_FEATURE(sciObj)->style);
          return SET_PROPERTY_ERROR;
        }
    }
  else
    {
      /* Wrong datatype */
      sciprint(_("FontWeight property value must be a single string: light, normal, demi or bold.\n"));
      return SET_PROPERTY_ERROR;
    }

}

