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
#include "UnitsConversion.hxx"

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
          
      /* If the parent is a Frame we have to modify Java returned values to get the real position of the uicontrol */
      if (sciGetEntityType(sciGetParent(sciObj)) == SCI_UICONTROL)
        {
          int * framePosition = NULL;

          framePosition = CallScilabBridge::getFramePosition(getScilabJavaVM(),
                                                             pUICONTROL_FEATURE(sciGetParent(sciObj))->hashMapIndex);
          returnValues[0] -= framePosition[0];
          returnValues[1] -= framePosition[1];

	  delete [] framePosition;
        }

      tmp = new double[4];
      tmp[0] = ConvertFromPixel((int) returnValues[0], pUICONTROL_FEATURE(sciObj)->units, sciObj, TRUE);
      tmp[1] = ConvertFromPixel((int) returnValues[1], pUICONTROL_FEATURE(sciObj)->units, sciObj, FALSE);
      tmp[2] = ConvertFromPixel((int) returnValues[2], pUICONTROL_FEATURE(sciObj)->units, sciObj, TRUE);
      tmp[3] = ConvertFromPixel((int) returnValues[3], pUICONTROL_FEATURE(sciObj)->units, sciObj, FALSE);
      
     
      returnFlag =  sciReturnRowVector(tmp, 4);
      
      delete [] tmp;
      delete [] returnValues;
      
      return returnFlag;
    }
  else
    {
      Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "Position");
      return FALSE;
    }
}

