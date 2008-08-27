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
		  int ret = 0;
		  char *text = CallScilabBridge::getFrameText(getScilabJavaVM(),pUICONTROL_FEATURE(sciObj)->hashMapIndex);
          ret = sciReturnString(text);
		  delete [] text;
		  return ret;
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
			  int l = 0;
			  char **texts = CallScilabBridge::getListBoxAllItemsText(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex);
			  int ret = 0;

              ret = sciReturnStringMatrix(texts,1, nbItems);

			  for (l = 0;l < nbItems; l++)
			  {
				  delete [] texts[l];
			  }
			  delete [] texts;

			  return ret;
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
			  int l = 0;
			  char **texts = CallScilabBridge::getPopupMenuAllItemsText(getScilabJavaVM(),pUICONTROL_FEATURE(sciObj)->hashMapIndex);
			  int ret = sciReturnStringMatrix(texts, 1, nbItems);

			  for (l = 0; l < nbItems; l++)
			  {
				delete [] texts[l];
			  }
			  delete [] texts;
              return ret;
            }
        }
      else/* All other uicontrol style */
        {
		  int ret = 0;
		  char *text = CallScilabBridge::getWidgetText(getScilabJavaVM(),pUICONTROL_FEATURE(sciObj)->hashMapIndex);
          ret = sciReturnString(text);
		  delete [] text;                                    
		  return ret;
        }
    }
  else
    {
      sciprint(_("No '%s' property for this object.\n"), "String");
      return FALSE;
    }
}
