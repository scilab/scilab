/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Sets the vertical alignment of an uicontrol object */

#include "SetUicontrolVerticalAlignment.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolVerticalAlignment(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  /* VerticalAlignment can be top, middle or bottom */
  
  char * alignment = NULL;

  if (valueType == sci_strings)
    {
      if(nbCol != 1 || nbRow == 0)
        {
          /* Wrong string size */
          sciprint(_("VerticalAlignment property value must be a single string: top, middle or bottom.\n"));
          return SET_PROPERTY_ERROR;
        }
      
      alignment = getStringFromStack(stackPointer);

      if (strcmp(alignment, "top") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->verticalAlignment = TOP_ALIGNMENT;
        }
      else if(strcmp(alignment, "middle") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->verticalAlignment = MIDDLE_ALIGNMENT;
        }
      else if(strcmp(alignment, "bottom") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->verticalAlignment = BOTTOM_ALIGNMENT;
        }
      else
        {
          /* Wrong string format */
          sciprint(_("VerticalAlignment property value must be a single string: top, middle or bottom.\n"));
          return SET_PROPERTY_ERROR;
        }
      
      // Update Java object
      if (pUICONTROL_FEATURE(sciObj)->style != SCI_FRAME)
        {
          CallScilabBridge::setWidgetVerticalAlignment(getScilabJavaVM(), 
                                                       pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                       alignment);
        }
      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      /* Wrong datatype */
      sciprint(_("VerticalAlignment property value must be a single string: top, middle or bottom.\n"));
      return SET_PROPERTY_ERROR;
    }

}

