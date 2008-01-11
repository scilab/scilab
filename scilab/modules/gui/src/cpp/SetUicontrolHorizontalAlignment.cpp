/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Sets the horizontal alignment of an uicontrol object */

#include "SetUicontrolHorizontalAlignment.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolHorizontalAlignment(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  /* HorizontalAlignment can be left, center or right */
  
  char * alignment = NULL;

  if (valueType == sci_strings)
    {
      if(nbCol != 1 || nbRow == 0)
        {
          /* Wrong string size */
          sciprint(_("HorizontalAlignment property value must be a single string: left, center or right.\n"));
          return SET_PROPERTY_ERROR;
        }
      
      alignment = getStringFromStack(stackPointer);

      if (strcmp(alignment, "left") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->horizontalAlignment = LEFT_ALIGNMENT;
        }
      else if(strcmp(alignment, "center") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->horizontalAlignment = CENTER_ALIGNMENT;
        }
      else if(strcmp(alignment, "right") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->horizontalAlignment = RIGHT_ALIGNMENT;
        }
      else
        {
          /* Wrong string format */
          sciprint(_("HorizontalAlignment property value must be a single string: left, center or right.\n"));
          return SET_PROPERTY_ERROR;
        }
      
      // Update Java object
      if (pUICONTROL_FEATURE(sciObj)->style != SCI_UIFRAME
          && pUICONTROL_FEATURE(sciObj)->style != SCI_SLIDER
          && pUICONTROL_FEATURE(sciObj)->style != SCI_POPUPMENU
          && pUICONTROL_FEATURE(sciObj)->style != SCI_LISTBOX)
        {
          CallScilabBridge::setWidgetHorizontalAlignment(getScilabJavaVM(), 
                                                       pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                         alignment);
        }
      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      /* Wrong datatype */
      sciprint(_("HorizontalAlignment property value must be a single string: left, center or right.\n"));
      return SET_PROPERTY_ERROR;
    }

}

