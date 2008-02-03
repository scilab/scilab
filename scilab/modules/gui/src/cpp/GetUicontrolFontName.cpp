/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the font name of an uicontrol */

#include "GetUicontrolFontName.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolFontName(sciPointObj* sciObj)
{
  if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      // Get the font name from Java
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrol */
        {
          return sciReturnString(CallScilabBridge::getFrameFontName(getScilabJavaVM(),
                                                                 pUICONTROL_FEATURE(sciObj)->hashMapIndex));
        }
      else/* All other uicontrol style */
        {
          return sciReturnString(CallScilabBridge::getWidgetFontName(getScilabJavaVM(),
                                                                 pUICONTROL_FEATURE(sciObj)->hashMapIndex));
        }
     }
  else
    {
      sciprint(_("No %s property for this object.\n"), "FontName");
      return FALSE;
    }
}

