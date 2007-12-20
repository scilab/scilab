/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the min property of an uicontrol */

#include "GetUicontrolMin.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolMin(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      return sciReturnDouble(pUICONTROL_FEATURE(sciObj)->min);
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "Min");
      return FALSE;
    }
}
