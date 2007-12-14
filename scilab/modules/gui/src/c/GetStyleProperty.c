/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the style of an uicontrol object */

#include "ObjectStructure.h"
#include "returnProperty.h"
#include "GetProperty.h"
#include "localization.h"

int GetStyleProperty(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      return sciReturnString(pUICONTROL_FEATURE(sciObj)->style);
    }
  else
    {
      Scierror(999, _("No style property for this object.\n"));
      return FALSE;
    }
  
}
