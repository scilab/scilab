/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the foreground color of an uicontrol or an uimenu */

#include "GetUiobjectForegroundColor.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUiobjectForegroundColor(sciPointObj* sciObj)
{
  int returnFlag = FALSE;
  long int * returnValues = NULL;
  double * tmp = NULL;

  if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      // Get the color from Java
      switch(pUICONTROL_FEATURE(sciObj)->style)
        {
        case SCI_PUSHBUTTON:
        case SCI_EDIT:
        case SCI_UITEXT:
          returnValues = CallScilabBridge::getWidgetForegroundColor(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex);
          break;
        default:
          /* Unimplemented uicontrol style */
          sciprint(_("No %s property for uicontrols of style: %s.\n"), "ForegroundColor", UicontrolStyleToString(pUICONTROL_FEATURE(sciObj)->style));
          return FALSE;
        }
    }
  else if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      returnValues = CallScilabBridge::getWidgetForegroundColor(getScilabJavaVM(), pUIMENU_FEATURE(sciObj)->hashMapIndex);
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "ForegroundColor");
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

