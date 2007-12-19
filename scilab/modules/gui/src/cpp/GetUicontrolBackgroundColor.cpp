/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the background color of an uicontrol */

#include "GetUicontrolBackgroundColor.hxx"
#include "CallScilabBridge.hxx"
extern "C"{
#include "getScilabJavaVM.h"
#include "GetProperty.h"
#include "localization.h"
}

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolBackgroundColor(sciPointObj* sciObj)
{
  int returnFlag = FALSE;
  long int * returnValues = NULL;
  double * tmp = NULL;

  if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      // Get the color from Java
      if(strcmp(pUICONTROL_FEATURE(sciObj)->style, "pushbutton")==0)
        {
          returnValues = CallScilabBridge::getPushButtonBackgroundColor(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex);
          
          tmp = new double[3];
          tmp[0] = (double)returnValues[0] / 255; // Red
          tmp[1] = (double)returnValues[1] / 255; // Green
          tmp[2] = (double)returnValues[2] / 255; // Blue
          
          returnFlag =  sciReturnRowVector(tmp, 3);
          
          delete(tmp);
          
          return returnFlag;
        }
      else
        {
          Scierror(999, _("No BackgroundColor property for uicontrols of style: %s.\n"), pUICONTROL_FEATURE(sciObj)->style);
          return FALSE;
        }
    }
  else
    {
      Scierror(999, _("No BackgroundColor property for this object.\n"));
      return FALSE;
    }
}

