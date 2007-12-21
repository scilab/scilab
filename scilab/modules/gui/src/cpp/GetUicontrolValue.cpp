/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the value property of an uicontrol */

#include "GetUicontrolValue.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolValue(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      if (pUICONTROL_FEATURE(sciObj)->valueSize == 0)
        {
          return sciReturnEmptyMatrix();
        }
      else
        {
          return sciReturnRowVectorFromInt(pUICONTROL_FEATURE(sciObj)->value, pUICONTROL_FEATURE(sciObj)->valueSize);
        }
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "Value");
      return FALSE;
    }
}
