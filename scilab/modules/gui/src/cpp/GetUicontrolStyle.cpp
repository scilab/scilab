/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the style of an uicontrol object */

#include "GetUicontrolStyle.hxx"

int GetUicontrolStyle(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      return sciReturnString(UicontrolStyleToString(pUICONTROL_FEATURE(sciObj)->style));
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "Style");
      return FALSE;
    }
  
}
