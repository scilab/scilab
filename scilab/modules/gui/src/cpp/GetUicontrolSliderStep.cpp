/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the SliderStep property of an uicontrol */

#include "GetUicontrolSliderStep.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolSliderStep(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      return sciReturnMatrix(pUICONTROL_FEATURE(sciObj)->sliderStep, 1, 2);
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "SliderStep");
      return FALSE;
    }
}
