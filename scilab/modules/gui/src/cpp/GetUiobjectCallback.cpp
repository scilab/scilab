/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the callback string of an uicontrol or uimenu */

#include "GetUiobjectCallback.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUiobjectCallback(sciPointObj* sciObj)
{
  if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      /* Do not read from Java because can be diffucult to get back the callback */

      if (pUIMENU_FEATURE(sciObj)->callback == NULL) /* Callback not set */
        {
          return sciReturnString("");
        }
      else /* Callback has been set */
        {
          return sciReturnString(pUIMENU_FEATURE(sciObj)->callback);
        }
    }
  else if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      /* Do not read from Java because can be diffucult to get back the callback */

      if (pUICONTROL_FEATURE(sciObj)->callback == NULL) /* Callback not set */
        {
          return sciReturnString("");
        }
      else /* Callback has been set */
        {
          return sciReturnString(pUICONTROL_FEATURE(sciObj)->callback);
        }
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "Callback");
      return FALSE;
    }
}

