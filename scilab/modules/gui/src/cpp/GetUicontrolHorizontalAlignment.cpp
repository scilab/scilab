/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the horizontal alignment property of an uicontrol */

#include "GetUicontrolHorizontalAlignment.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolHorizontalAlignment(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      switch(pUICONTROL_FEATURE(sciObj)->horizontalAlignment)
        {
        case LEFT_ALIGNMENT:
          return sciReturnString("left");
        case CENTER_ALIGNMENT:
          return sciReturnString("center");
        case RIGHT_ALIGNMENT:
          return sciReturnString("right");
        default:
          sciprint(_("HorizontalAlignment property value must be a single string: left, center, or right.\n"));
          return FALSE;
        }
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "HorizontalAlignment");
      return FALSE;
    }
}
