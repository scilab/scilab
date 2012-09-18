 /*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolStyle.hxx"

extern "C"
{
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
}

/**
 ** \brief Get the style of an uicontrol object
 */

static char *IntToStyle(int _iStyle)
{
    switch (_iStyle)
    {
    case __GO_UI_CHECKBOX__ :
        return "CheckBox";
    case __GO_UI_EDIT__ :
    case __GO_UI_FRAME__ :
    case __GO_UI_IMAGE__ :
    case __GO_UI_LISTBOX__ :
    case __GO_UI_POPUPMENU__ :
    case __GO_UI_PUSHBUTTON__ :
    case __GO_UI_RADIOBUTTON__ :
    case __GO_UI_SLIDER__ :
    case __GO_UI_TABLE__ :
    case __GO_UI_TEXT__ :
        return "UI_????";
    }
}

int GetUicontrolStyle(void* _pvCtx, char *sciObjUID)
{
    int iStyle = -1;
    int *piStyle = &iStyle;

    getGraphicObjectProperty(sciObjUID, __GO_STYLE__, jni_int, (void **) &piStyle);

  if (piStyle != NULL)
  {
      return sciReturnString(_pvCtx, IntToStyle(iStyle));
  }
  else
  {
      Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "Style");
      return FALSE;
  }

}
