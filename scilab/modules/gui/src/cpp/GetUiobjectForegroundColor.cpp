/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the foreground color of an uicontrol or an uimenu */

#include "GetUiobjectForegroundColor.hxx"
#include "CallScilabBridge.hxx"
extern "C"{
#include "getScilabJavaVM.h"
#include "GetProperty.h"
#include "localization.h"
}

using namespace org_scilab_modules_gui_bridge;

int GetUiobjectForegroundColor(sciPointObj* sciObj)
{
  int returnFlag = FALSE;
  long int * returnValues = NULL;
  double * tmp = NULL;

  if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      // Get the color from Java
      if(strcmp(pUICONTROL_FEATURE(sciObj)->style, "pushbutton")==0)
        {
          returnValues = CallScilabBridge::getPushButtonForegroundColor(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex);
        }
      else
        {
          /* Unimplemented uicontrol style */
          sciprint(_("No ForegroundColor property for uicontrols of style: %s.\n"), pUICONTROL_FEATURE(sciObj)->style);
          return FALSE;
        }
    }
  else if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      returnValues = CallScilabBridge::getMenuForegroundColor(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex);
    }
  else
    {
      sciprint(_("No ForegroundColor property for this object.\n"));
      return FALSE;
    }
          
  /* Format returned data */
  tmp = new double[3];
  tmp[0] = (double)returnValues[0] / 255; // Red
  tmp[1] = (double)returnValues[1] / 255; // Green
  tmp[2] = (double)returnValues[2] / 255; // Blue
  
  returnFlag =  sciReturnRowVector(tmp, 3);
  
  delete(tmp);
  
  return returnFlag;
}

