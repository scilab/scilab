/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the position of an uicontrol */

#include "GetUicontrolPosition.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolPosition(sciPointObj* sciObj)
{
  int returnFlag = FALSE;
  long int * returnValues = NULL;
  double * tmp = NULL;

  if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      // Get the position from Java
      returnValues = CallScilabBridge::getWidgetPosition(getScilabJavaVM(),
                                                         pUICONTROL_FEATURE(sciObj)->hashMapIndex);
          
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
      sciprint(_("No %s property for this object.\n"), "Position");
      return FALSE;
    }
}

