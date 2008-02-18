/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Get the string of an uicontrol
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolString.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolString(sciPointObj* sciObj)
{  
  int nbItems = 0;

  if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      // Get the string from Java
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrol */
        {
          return sciReturnString(CallScilabBridge::getFrameText(getScilabJavaVM(),
                                                                 pUICONTROL_FEATURE(sciObj)->hashMapIndex));
        }
      else if (pUICONTROL_FEATURE(sciObj)->style == SCI_LISTBOX) /* ListBox style uicontrol */
        {
          /* Return a row vector */
          nbItems = CallScilabBridge::getListBoxNumberOfItems(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex);
          if (nbItems == 0) /* If no Items, the return matrix is an empty string of size 1x1 */
            {
              return sciReturnString("");
            }
          else 
            {
              return sciReturnStringMatrix(CallScilabBridge::getListBoxAllItemsText(getScilabJavaVM(),
                                                                                    pUICONTROL_FEATURE(sciObj)->hashMapIndex),
                                           1,
                                           nbItems);
            }
        }
      else if (pUICONTROL_FEATURE(sciObj)->style == SCI_POPUPMENU) /* PopupMenu style uicontrol */
        {
          /* Return a row vector */
          nbItems = CallScilabBridge::getPopupMenuNumberOfItems(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex);
          if (nbItems == 0) /* If no Items, the return matrix is an empty string of size 1x1 */
            {
              return sciReturnString("");
            }
          else
            {
              return sciReturnStringMatrix(CallScilabBridge::getPopupMenuAllItemsText(getScilabJavaVM(),
                                                                                      pUICONTROL_FEATURE(sciObj)->hashMapIndex),
                                           1,
                                           nbItems);
            }
        }
      else/* All other uicontrol style */
        {
          return sciReturnString(CallScilabBridge::getWidgetText(getScilabJavaVM(),
                                                                 pUICONTROL_FEATURE(sciObj)->hashMapIndex));
        }
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "String");
      return FALSE;
    }
}
