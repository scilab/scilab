/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the vertical alignment property of an uicontrol */

#include "GetUicontrolVerticalAlignment.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolVerticalAlignment(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      switch(pUICONTROL_FEATURE(sciObj)->verticalAlignment)
        {
        case TOP_ALIGNMENT:
          return sciReturnString("top");
        case MIDDLE_ALIGNMENT:
          return sciReturnString("middle");
        case BOTTOM_ALIGNMENT:
          return sciReturnString("bottom");
        default:
          sciprint(_("VerticalAlignment property value must be a single string: top, middle, or bottom.\n"));
          return FALSE;
        }
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "VerticalAlignment");
      return FALSE;
    }
}
