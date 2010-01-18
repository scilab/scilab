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

#include "DestroyJavaUiobject.hxx"

using namespace org_scilab_modules_gui_bridge;

void DestroyJavaUiobject(sciPointObj * sciObj)
{
  if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrol */
        {
          CallScilabBridge::destroyFrame(getScilabJavaVM(),
                                         pUICONTROL_FEATURE(sciObj)->hashMapIndex);
        }
      else
        {
          CallScilabBridge::destroyWidget(getScilabJavaVM(),
                                         pUICONTROL_FEATURE(sciObj)->hashMapIndex);
        }
    }
  else if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
          CallScilabBridge::destroyWidget(getScilabJavaVM(),
                                         pUIMENU_FEATURE(sciObj)->hashMapIndex);
    }
  else
    {
		sciprint(const_cast<char*>(_("%s: Could not destroy this object.\n")), "DestroyJavaUiobject");
    }
}
