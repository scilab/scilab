/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "ScilabView.hxx"

extern "C"
{
#include <string.h>
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

#include "BOOL.h"
#include "CurrentObject.h"
}


int getCurrentObject(void)
{
    return ScilabView::getCurrentObject();
}

void setCurrentObject(int UID)
{
    ScilabView::setCurrentObject(UID);
}

BOOL isCurrentObject(int UID)
{
    if (UID == ScilabView::getCurrentObject())
    {
        return TRUE;
    }
    return FALSE;
}

int getParentObject(int iUID)
{
    int iParent = 0;
    int* piParent = &iParent;
    if (iUID == 0)
    {
        return 0;
    }

    getGraphicObjectProperty(iUID, __GO_PARENT__, jni_int, (void**)&piParent);
    return iParent;
}

void setParentObject(int iUID, int iParent)
{
    setGraphicObjectProperty(iUID, __GO_PARENT__, &iParent, jni_int, 1);
    return;
}
