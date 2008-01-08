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
    case SCI_EDIT:
      return "edit";
    default:
      return "Unknown";
    }
}
