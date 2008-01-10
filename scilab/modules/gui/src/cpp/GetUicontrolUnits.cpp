/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the units of an uicontrol */

#include "GetUicontrolUnits.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolUnits(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      // Get the font units from Scilab object
      switch(pUICONTROL_FEATURE(sciObj)->units)
        {
        case POINTS_UNITS:
          return sciReturnString("points");
        case NORMALIZED_UNITS:
          return sciReturnString("normalized");
        case INCHES_UNITS:
          return sciReturnString("inches");
        case CENTIMETERS_UNITS:
          return sciReturnString("centimeters");
        case PIXELS_UNITS:
          return sciReturnString("pixels");
        default:
          sciprint(_("Units property value must be a single string: points, normalized, inches, centimeters or pixels.\n"));
          return FALSE;
        }
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "Units");
      return FALSE;
    }
}
