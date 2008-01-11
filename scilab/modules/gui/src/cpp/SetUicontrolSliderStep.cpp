/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Sets the slider step property of an uicontrol object */

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
          CallScilabBridge::setSliderMinorTickSpacing(getScilabJavaVM(),
                                                       pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                       (int) pUICONTROL_FEATURE(sciObj)->sliderStep[0] * (pUICONTROL_FEATURE(sciObj)->max - pUICONTROL_FEATURE(sciObj)->min));

           CallScilabBridge::setSliderMajorTickSpacing(getScilabJavaVM(), 
                                                       pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                       (int) pUICONTROL_FEATURE(sciObj)->sliderStep[1] * (pUICONTROL_FEATURE(sciObj)->max - pUICONTROL_FEATURE(sciObj)->min));
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

