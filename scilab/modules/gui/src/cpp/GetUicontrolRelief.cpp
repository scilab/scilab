/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the relief property of an uicontrol */

#include "GetUicontrolRelief.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolRelief(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      switch(pUICONTROL_FEATURE(sciObj)->relief)
        {
        case FLAT_RELIEF:
          return sciReturnString("flat");
        case GROOVE_RELIEF:
          return sciReturnString("groove");
        case RAISED_RELIEF:
          return sciReturnString("raised");
        case RIDGE_RELIEF:
          return sciReturnString("ridge");
        case SOLID_RELIEF:
          return sciReturnString("solid");
        case SUNKEN_RELIEF:
          return sciReturnString("sunken");
        default:
          sciprint(_("Relief property value must be a single string: flat, groove, raised, ridge, solid, or sunken.\n"));
          return FALSE;
        }
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "Relief");
      return FALSE;
    }
}
