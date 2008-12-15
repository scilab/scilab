/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Sets the slider step property of an uicontrol object  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolSliderStep.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolSliderStep(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  double *allValues = NULL;
  
  if (sciGetEntityType( sciObj ) != SCI_UICONTROL)
    {
      Scierror(999, _("No '%s' property for this object.\n"), "SliderStep");
      return SET_PROPERTY_ERROR;
    }

  if (valueType == sci_matrix)
    {
      if((nbRow > 1) || (nbCol != 2))
        {
          /* Wrong value size */
          Scierror(999, _("Wrong size for '%s' property: A 1 x %d real row vector expected.\n"), "SliderStep", 2);
          return SET_PROPERTY_ERROR;
        }
      
      /* Store the value in Scilab */
      allValues = getDoubleMatrixFromStack(stackPointer);

      /* SliderStep field is allocated if necessary */
      if (pUICONTROL_FEATURE(sciObj)->sliderStep == NULL)
        {
          pUICONTROL_FEATURE(sciObj)->sliderStep = new double[2];
        }
      pUICONTROL_FEATURE(sciObj)->sliderStep[0] = allValues[0];
      pUICONTROL_FEATURE(sciObj)->sliderStep[1] = allValues[1];

      // Set the Java object property for sliders
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_SLIDER)
        {
          /* BEGIN If sliderstep is a value */
          CallScilabBridge::setSliderMinorTickSpacing(getScilabJavaVM(),
                                                      pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                      (int) pUICONTROL_FEATURE(sciObj)->sliderStep[0]);
          
          CallScilabBridge::setSliderMajorTickSpacing(getScilabJavaVM(), 
                                                      pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                      (int) pUICONTROL_FEATURE(sciObj)->sliderStep[1]);
          /* END If sliderstep is a value */
        }
      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      /* Wrong datatype */
      Scierror(999, _("Wrong type for '%s' property: A 1 x %d real row vector expected.\n"), "SliderStep", 2);
      return SET_PROPERTY_ERROR;
    }

}

