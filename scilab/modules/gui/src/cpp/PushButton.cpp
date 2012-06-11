/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "PushButton.hxx"

extern "C"
{
#include "BuildObjects.h"
#include "CurrentFigure.h"
#include "setGraphicObjectProperty.h"
}

using namespace org_scilab_modules_gui_bridge;

void createPushButton(sciPointObj* sciObj)
{
    CallScilabBridge::newPushButton(getScilabJavaVM(), sciObj->UID);
}

int setCurentFigureAsPushButtonParent(char *sciObjUID)
{
    // Scilab list of parent
    //sciAddThisToItsParent(sciObj, sciGetCurrentFigure());

    char const* pstCurrentFigure = getCurrentFigure();
    if (pstCurrentFigure == NULL)
    {
        pstCurrentFigure = createNewFigureWithAxes();
    }
    setGraphicObjectRelationship(pstCurrentFigure, sciObjUID);


    // Java objects
    //CallScilabBridge::setPushButtonParent(getScilabJavaVM(), pobjCurrentFigure->UID, sciObj->UID);

    return SET_PROPERTY_SUCCEED;
}
