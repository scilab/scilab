/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Sets the relief of an uicontrol object */

#include "SetUicontrolRelief.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolRelief(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  /* Relief can be flat, groove, raised, ridge, solid or sunken */

  char * relief = NULL;
  
  if (valueType == sci_strings)
    {
      if(nbCol != 1 || nbRow == 0)
        {
          /* Wrong string size */
          sciprint(_("Relief property value must be a single string: flat, groove, raised, ridge, solid or sunken.\n"));
          return SET_PROPERTY_ERROR;
        }
      
      relief = getStringFromStack(stackPointer);

      if (strcmp(relief, "flat") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->relief = FLAT_RELIEF;
        }
      else if(strcmp(relief, "groove") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->relief = GROOVE_RELIEF;
        }
      else if(strcmp(relief, "raised") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->relief = RAISED_RELIEF;
        }
      else if(strcmp(relief, "ridge") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->relief = RIDGE_RELIEF;
        }
      else if(strcmp(relief, "solid") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->relief = SOLID_RELIEF;
        }
      else if(strcmp(relief, "sunken") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->relief = SUNKEN_RELIEF;
        }
      else
        {
          /* Wrong string format */
          sciprint(_("Relief property value must be a single string: flat, groove, raised, ridge, solid or sunken.\n"));
          return SET_PROPERTY_ERROR;
        }
      
      switch(pUICONTROL_FEATURE(sciObj)->style)
        {
        case SCI_PUSHBUTTON:
          // TODO Change the size of the Java object
           return SET_PROPERTY_SUCCEED;
        default:
          /* Unimplemented uicontrol style */
          sciprint(_("No %s property for uicontrols of style: %s.\n"), "Relief", UicontrolStyleToString(pUICONTROL_FEATURE(sciObj)->style));
          return SET_PROPERTY_ERROR;
        }
    }
  else
    {
      /* Wrong datatype */
          sciprint(_("Relief property value must be a single string: flat, groove, raised, ridge, solid or sunken.\n"));
      return SET_PROPERTY_ERROR;
    }

}

