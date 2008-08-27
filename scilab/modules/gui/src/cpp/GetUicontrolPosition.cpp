/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Get the position of an uicontrol 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolPosition.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolPosition(sciPointObj* sciObj)
{
  int returnFlag = FALSE;
  int * returnValues = NULL;
  double * tmp = NULL;

  if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      // Get the position from Java
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrol */
        {
          returnValues = CallScilabBridge::getFramePosition(getScilabJavaVM(),
                                                             pUICONTROL_FEATURE(sciObj)->hashMapIndex);
        }
      else /* All other uicontrol styles */
        {
          returnValues = CallScilabBridge::getWidgetPosition(getScilabJavaVM(),
                                                             pUICONTROL_FEATURE(sciObj)->hashMapIndex);
        }
          
      tmp = new double[4];
      tmp[0] = returnValues[0];
      tmp[1] = returnValues[1];
      tmp[2] = returnValues[2];
      tmp[3] = returnValues[3];
     
      returnFlag =  sciReturnRowVector(tmp, 4);
      
      delete [] tmp;
	  delete [] returnValues;
      
      return returnFlag;
    }
  else
    {
      sciprint(_("No '%s' property for this object.\n"), "Position");
      return FALSE;
    }
}

