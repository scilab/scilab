/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
*
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUiobjectTag.hxx"

extern "C" {
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

using namespace org_scilab_modules_gui_bridge;

// TODO : move to graphics/src/c/getHandleProperty

int GetUiobjectTag(sciPointObj* sciObj)
{
    char *pstrObjectTag;
    getGraphicObjectProperty(sciObj->UID, __GO_TAG__, jni_string, (void **)&pstrObjectTag);

    if (pstrObjectTag == NULL)
    {
        Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "Tag");
        return FALSE;
    }

    return sciReturnString(pstrObjectTag);
}

