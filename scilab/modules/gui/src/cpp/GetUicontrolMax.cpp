/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the max property of an uicontrol */

#include "GetUicontrolMax.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolMax(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      return sciReturnDouble(pUICONTROL_FEATURE(sciObj)->max);
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "Max");
      return FALSE;
    }
}
