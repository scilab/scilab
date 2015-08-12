/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Get the position of an uicontrol
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

extern "C"
{
#include "GetUicontrol.h"
}

int GetUicontrolPosition(void* _pvCtx, int iObjUID)
{
    double* position = NULL;
    int status = FALSE;

    getGraphicObjectProperty(iObjUID, __GO_POSITION__, jni_double_vector, (void**) &position);

    if (position == NULL)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Position");
        return FALSE;
    }
    else
    {
        status = sciReturnRowVector(_pvCtx, position, 4);
        delete[] position;
        return status;
    }
}

