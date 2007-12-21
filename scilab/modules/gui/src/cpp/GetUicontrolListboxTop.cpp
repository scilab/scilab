/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the listbox top property of an uicontrol */

#include "GetUicontrolListboxTop.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolListboxTop(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      if (pUICONTROL_FEATURE(sciObj)->listboxTop == NULL)
        {
          return sciReturnEmptyMatrix();
        }
      else
        {
          return sciReturnRowVectorFromInt(pUICONTROL_FEATURE(sciObj)->listboxTop, 1);
        }
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "ListboxTop");
      return FALSE;
    }
}
