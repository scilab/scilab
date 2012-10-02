/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Sets the relief of an uicontrol object
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolRelief.hxx"

int SetUicontrolRelief(void* _pvCtx, char* sciObjUID, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
    /* Relief can be flat, groove, raised, ridge, solid or sunken */

    char* relief = NULL;
    BOOL status = FALSE;

    if (valueType != sci_strings)
    {
        /* Wrong datatype */
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: '%s', '%s', '%s', '%s', '%s' or '%s' expected.\n")), "Relief", "flat", "groove", "raised", "ridge", "solid", "sunken");
        return SET_PROPERTY_ERROR;
    }

    if(nbCol != 1 || nbRow == 0)
    {
        /* Wrong string size */
        Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: '%s', '%s', '%s', '%s', '%s' or '%s' expected.\n")), "Relief", "flat", "groove", "raised", "ridge", "solid", "sunken");
        return SET_PROPERTY_ERROR;
    }

      relief = getStringFromStack(stackPointer);

      if (stricmp(relief, "flat") != 0
          && stricmp(relief, "groove") != 0
          && stricmp(relief, "raised") != 0
          && stricmp(relief, "ridge") != 0
          && stricmp(relief, "solid") != 0
          && stricmp(relief, "sunken") != 0)
      {
          /* Wrong string format */
          Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: '%s', '%s', '%s', '%s', '%s' or '%s' expected.\n")), "Relief", "flat", "groove", "raised", "ridge", "solid", "sunken");
          return SET_PROPERTY_ERROR;
      }

    status = setGraphicObjectProperty(sciObjUID, __GO_UI_RELIEF__, relief, jni_string, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Relief");
        return SET_PROPERTY_ERROR;
    }
}
