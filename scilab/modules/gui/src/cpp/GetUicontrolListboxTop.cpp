/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2010-2011 - DIGITEO - Vincent COUVERT
 *
 * Get the listbox top property of an uicontrol
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolListboxTop.hxx"

int GetUicontrolListboxTop(void* _pvCtx, char *sciObjUID)
{
    int listboxTopSize = 0;
    int* piListboxTopSize = &listboxTopSize;
    int* piListboxTop = NULL;;

    getGraphicObjectProperty(sciObjUID, __GO_UI_LISTBOXTOP_SIZE__, jni_int, (void**) &piListboxTopSize);

    if (piListboxTopSize == NULL)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "ListboxTop");
        return FALSE;
    }
    else
    {
        if (listboxTopSize == 0)
        {
          return sciReturnEmptyMatrix(_pvCtx);
        }
        else
        {
            getGraphicObjectProperty(sciObjUID, __GO_UI_LISTBOXTOP__, jni_int_vector, (void**) &piListboxTop);

            if (piListboxTop == NULL)
            {
                Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "ListboxTop");
                return FALSE;
            }
            else
            {
                return sciReturnRowVectorFromInt(_pvCtx, piListboxTop, listboxTopSize);
            }
        }
    }
}
