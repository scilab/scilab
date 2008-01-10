/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Return the style name corresponding to the integer value from UicontrolStyle enum */

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
      return "popup";
    default:
      return "Unknown";
    }
}
