/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Get the foreground color of an uicontrol or an uimenu 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUiobjectForegroundColor.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUiobjectForegroundColor(sciPointObj* sciObj)
{
  int returnFlag = FALSE;
  int * returnValues = NULL;
  double * tmp = NULL;

  if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      // Get the color from Java
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrol */
        {
          returnValues = CallScilabBridge::getFrameForegroundColor(getScilabJavaVM(),
                                                                    pUICONTROL_FEATURE(sciObj)->hashMapIndex);
        }
      else /* All other uicontrol style */
        {
          returnValues = CallScilabBridge::getWidgetForegroundColor(getScilabJavaVM(),
                                                                    pUICONTROL_FEATURE(sciObj)->hashMapIndex);
        }
    }
  else if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      returnValues = CallScilabBridge::getWidgetForegroundColor(getScilabJavaVM(), pUIMENU_FEATURE(sciObj)->hashMapIndex);
    }
  else
    {
      Scierror(999, _("No '%s' property for this object.\n"), "ForegroundColor");
      return FALSE;
    }
          
  /* Format returned data */
  tmp = new double[3];
  
  /* If values are stored in Scilab then they are read */
  /* else they are writen */
  if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      if (pUICONTROL_FEATURE(sciObj)->foregroundcolor != NULL)
        {
          tmp[0] = pUICONTROL_FEATURE(sciObj)->foregroundcolor[0];
          tmp[1] = pUICONTROL_FEATURE(sciObj)->foregroundcolor[1];
          tmp[2] = pUICONTROL_FEATURE(sciObj)->foregroundcolor[2];
        }
      else
        {
          tmp[0] = (double)returnValues[0] / 255; // Red
          tmp[1] = (double)returnValues[1] / 255; // Green
          tmp[2] = (double)returnValues[2] / 255; // Blue
          
          pUICONTROL_FEATURE(sciObj)->foregroundcolor = new double[3];
          pUICONTROL_FEATURE(sciObj)->foregroundcolor[0] = tmp[0];
          pUICONTROL_FEATURE(sciObj)->foregroundcolor[1] = tmp[1];
          pUICONTROL_FEATURE(sciObj)->foregroundcolor[2] = tmp[2];
        }
    }
  else if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      if (pUIMENU_FEATURE(sciObj)->foregroundcolor != NULL)
        {
          tmp[0] = pUIMENU_FEATURE(sciObj)->foregroundcolor[0];
          tmp[1] = pUIMENU_FEATURE(sciObj)->foregroundcolor[1];
          tmp[2] = pUIMENU_FEATURE(sciObj)->foregroundcolor[2];
        }
      else
        {
          tmp[0] = (double)returnValues[0] / 255; // Red
          tmp[1] = (double)returnValues[1] / 255; // Green
          tmp[2] = (double)returnValues[2] / 255; // Blue
          
          pUIMENU_FEATURE(sciObj)->foregroundcolor = new double[3];
          pUIMENU_FEATURE(sciObj)->foregroundcolor[0] = tmp[0];
          pUIMENU_FEATURE(sciObj)->foregroundcolor[1] = tmp[1];
          pUIMENU_FEATURE(sciObj)->foregroundcolor[2] = tmp[2];
        }
    }
  
  returnFlag =  sciReturnRowVector(tmp, 3);
  
  delete [] tmp;
  delete [] returnValues;
  
  return returnFlag;
}

