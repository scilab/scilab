/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
