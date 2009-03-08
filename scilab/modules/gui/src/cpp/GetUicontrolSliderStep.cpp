/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 * Get the SliderStep property of an uicontrol
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolSliderStep.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolSliderStep(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      if (pUICONTROL_FEATURE(sciObj)->sliderStep == NULL) /* No user defined value */
        {
          double sliderStep[2];
          sliderStep[0] = 0.01 * (pUICONTROL_FEATURE(sciObj)->max - pUICONTROL_FEATURE(sciObj)->min);
          sliderStep[1] = 0.1 * (pUICONTROL_FEATURE(sciObj)->max - pUICONTROL_FEATURE(sciObj)->min);
          return sciReturnMatrix(sliderStep, 1, 2);
        }
      else
        {
          return sciReturnMatrix(pUICONTROL_FEATURE(sciObj)->sliderStep, 1, 2);
        }
    }
  else
    {
      Scierror(999, _("No '%s' property for this object.\n"), "SliderStep");
      return FALSE;
    }
}
