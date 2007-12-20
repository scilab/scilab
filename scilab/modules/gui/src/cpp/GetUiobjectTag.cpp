/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the tag of an uicontrol or uimenu */

#include "GetUiobjectTag.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUiobjectTag(sciPointObj* sciObj)
{
  if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      // Get the label
      if (pUIMENU_FEATURE(sciObj)->tag == NULL)
        {
          return sciReturnString("");
        }
      else
        {
          return sciReturnString(pUIMENU_FEATURE(sciObj)->tag);
        }
    }
  else if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      // Get the label from Java
      switch(pUICONTROL_FEATURE(sciObj)->style)
        {
        case SCI_PUSHBUTTON:
          // Get the label
          if (pUICONTROL_FEATURE(sciObj)->tag == NULL)
            {
              return sciReturnString("");
            }
          else
            {
              return sciReturnString(pUICONTROL_FEATURE(sciObj)->tag);
            }
        default:
          sciprint(_("No %s property for uicontrols of style: %s.\n"), "tag", UicontrolStyleToString(pUICONTROL_FEATURE(sciObj)->style));
          return FALSE;
        }
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "Tag");
      return FALSE;
    }
}

