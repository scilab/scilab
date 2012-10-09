/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011-2012 - DIGITEO - Vincent COUVERT
 * Get the value property of an uicontrol
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolValue.hxx"

int GetUicontrolValue(void* _pvCtx, char *sciObjUID)
{
    int valueSize = 0;
    int* piValueSize = &valueSize;
    double* pdblValue = NULL;
    int status = 0;

    getGraphicObjectProperty(sciObjUID, __GO_UI_VALUE_SIZE__, jni_int, (void**) &piValueSize);

    if (piValueSize == NULL)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Value");
        return FALSE;
    }
    else
    {
        if (valueSize == 0)
        {
            return sciReturnEmptyMatrix(_pvCtx);
        }
        else
        {
            getGraphicObjectProperty(sciObjUID, __GO_UI_VALUE__, jni_double_vector, (void**) &pdblValue);

            if (pdblValue == NULL)
            {
                Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Value");
                return FALSE;
            }
            else
            {
                status = sciReturnRowVector(_pvCtx, pdblValue, valueSize);
                delete[] pdblValue;
                return status;
            }
        }
    }
    return status;
}
