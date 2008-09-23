/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Return the style name corresponding to the int value from UicontrolStyle enum
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "UicontrolStyleToString.h"
#include "ObjectStructure.h"

char * UicontrolStyleToString(int style)
{
  switch(style)
    {
    case SCI_PUSHBUTTON:
      return "pushbutton";
    case SCI_RADIOBUTTON:
      return "radiobutton";
    case SCI_CHECKBOX:
      return "checkbox";
    case SCI_EDIT:
      return "edit";
    case SCI_UITEXT:
      return "text";
    case SCI_SLIDER:
      return "slider";
    case SCI_UIFRAME:
      return "frame";
    case SCI_LISTBOX:
      return "listbox";
    case SCI_POPUPMENU:
      return "popupmenu";
    default:
      return "Unknown";
    }
}
