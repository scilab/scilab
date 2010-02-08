/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Destroy Java object matching Scilab uicontrol 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DestroyWaitBar.hxx"

extern "C"
{
#include "DestroyObjects.h"
}
using namespace org_scilab_modules_gui_bridge;

void DestroyWaitBar(sciPointObj * sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_WAITBAR)
    {
      CallScilabBridge::destroyWaitBar(getScilabJavaVM(),
                                       pWAITBAR_FEATURE(sciObj)->hashMapIndex);
    }
  else if (sciGetEntityType(sciObj) == SCI_PROGRESSIONBAR)
    {
      CallScilabBridge::destroyWaitBar(getScilabJavaVM(),
                                       pPROGRESSIONBAR_FEATURE(sciObj)->hashMapIndex);
    }
  else
    {
		sciprint(const_cast<char*>(_("%s: Could not destroy this object.\n")), "DestroyWaitBar");
    }

	sciStandardDestroyOperations(sciObj) ;
}
