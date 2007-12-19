/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the style of an uicontrol object */

#include "GetStyleProperty.h"
#include "ObjectStructure.h"
#include "returnProperty.h"
#include "GetProperty.h"
#include "localization.h"
#include "UicontrolStyleToString.h"

int GetStyleProperty(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      return sciReturnString(UicontrolStyleToString(pUICONTROL_FEATURE(sciObj)->style));
    }
  else
    {
      Scierror(999, _("No style property for this object.\n"));
      return FALSE;
    }
  
}
