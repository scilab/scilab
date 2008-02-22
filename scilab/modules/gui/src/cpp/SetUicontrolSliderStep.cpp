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

int SetUicontrolSliderStep(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  double *allValues = NULL;
  
  if (valueType == sci_matrix)
    {
      if((nbRow > 1) || (nbCol != 2))
        {
          /* Wrong value size */
          sciprint(_("%s property value must be a 2-value row vector.\n"), "SliderStep");
          return SET_PROPERTY_ERROR;
        }
      
      /* Store the value in Scilab */
      allValues = getDoubleMatrixFromStack(stackPointer);
      /* SliderStep field is allocated when object is created */
      pUICONTROL_FEATURE(sciObj)->sliderStep[0] = allValues[0];
      pUICONTROL_FEATURE(sciObj)->sliderStep[1] = allValues[1];

      // Set the Java object property for sliders
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_SLIDER)
        {
          /* BEGIN If sliderstep is a percentage */
          //CallScilabBridge::setSliderMinorTickSpacing(getScilabJavaVM(),
          //                                            pUICONTROL_FEATURE(sciObj)->hashMapIndex,
          //                                            (int) pUICONTROL_FEATURE(sciObj)->sliderStep[0] * (pUICONTROL_FEATURE(sciObj)->max - pUICONTROL_FEATURE(sciObj)->min));
          
          //CallScilabBridge::setSliderMajorTickSpacing(getScilabJavaVM(), 
          //                                            pUICONTROL_FEATURE(sciObj)->hashMapIndex,
          //                                            (int) pUICONTROL_FEATURE(sciObj)->sliderStep[1] * (pUICONTROL_FEATURE(sciObj)->max - pUICONTROL_FEATURE(sciObj)->min));
          /* END If sliderstep is a percentage */

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
      sciprint(_("%s property value must be a 2-value row vector.\n"), "SliderStep");
      return SET_PROPERTY_ERROR;
    }

}

