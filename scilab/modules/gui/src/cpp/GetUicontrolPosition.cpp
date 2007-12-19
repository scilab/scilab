/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the position of an uicontrol */

#include "GetUicontrolPosition.hxx"
#include "CallScilabBridge.hxx"
extern "C"{
#include "getScilabJavaVM.h"
#include "GetProperty.h"
#include "localization.h"
}

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolPosition(sciPointObj* sciObj)
{
  int returnFlag = FALSE;
  long int * returnValues = NULL;
  double * tmp = NULL;

  if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      // Get the position from Java
      if(strcmp(pUICONTROL_FEATURE(sciObj)->style, "pushbutton")==0)
        {
          returnValues = CallScilabBridge::getPushButtonPosition(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex);
          
          tmp = new double[4];
          tmp[0] = returnValues[0];
          tmp[1] = returnValues[1];
          tmp[2] = returnValues[2];
          tmp[3] = returnValues[3];
          
          returnFlag =  sciReturnRowVector(tmp, 4);
          
          delete(tmp);
          
          return returnFlag;
        }
      else
        {
          sciprint(_("No %s property for uicontrols of style: %s.\n"), "Position", pUICONTROL_FEATURE(sciObj)->style);
          return FALSE;
        }
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "Position");
      return FALSE;
    }
}

