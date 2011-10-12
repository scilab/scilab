/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 * Copyright (C) 2008 - Yung-Jang Lee
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "InitUIMenu.hxx"
#include "getHandleDrawer.h"
#include "CallScilabBridge.hxx"
extern "C"
{
#include "sci_types.h"
#include "BOOL.h"
#include "getScilabJavaVM.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"
#include "getPropertyAssignedValue.h"
#include "localization.h"
#include "Scierror.h"
#include "HandleManagement.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentFigure.h"
#include "BuildObjects.h"
}

using namespace org_scilab_modules_gui_bridge;

void InitUIMenu(sciPointObj * sciObj)
{
    pUIMENU_FEATURE(sciObj)->hashMapIndex = CallScilabBridge::newMenu(getScilabJavaVM());
}

int setMenuParent(char *pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
    char *pstCurrentFigure = NULL;
    char *parentType = NULL;
    char *pParentUID = NULL;

    /* Special case to set current figure for parent */
    if (stackPointer == -1)
    {
        // Set the parent property
        pstCurrentFigure = getCurrentFigure();
        if (pstCurrentFigure == NULL)
        {
            pstCurrentFigure = createNewFigureWithAxes();
        }
        setGraphicObjectRelationship(pstCurrentFigure, pobjUID);
        return SET_PROPERTY_SUCCEED;
    }

    if (nbRow * nbCol != 1)
    {
        // Parent must be a single value
        return SET_PROPERTY_ERROR;
    }

    if (valueType == sci_handles)
    {
        pParentUID = getObjectFromHandle(getHandleFromStack(stackPointer));
        if (pParentUID != NULL)
        {
            getGraphicObjectProperty(pParentUID, __GO_TYPE__, jni_string, (void **)&parentType);
            if ((strcmp(parentType, __GO_FIGURE__) == 0) || (strcmp(parentType, __GO_UIMENU__) == 0))   // || (strcmp(parentType, __GO_UICONTEXTMENU__) == 0) )
            {
                setGraphicObjectRelationship(pParentUID, pobjUID);
                free(parentType);
            }
            else
            {
                Scierror(999, const_cast < char *>(_("%s: Wrong type for parent: Figure or uimenu expected.\n")), "SetMenuParent");

                free(parentType);
                return SET_PROPERTY_ERROR;
            }
        }
        else
        {
            Scierror(999, const_cast < char *>(_("%s: Wrong type for parent: Figure or uimenu expected.\n")), "SetMenuParent");

            return SET_PROPERTY_ERROR;
        }
    }
    else
    {
        abort;
    }
#if 0
    if (valueType == sci_matrix)
    {
        // The parent is Scilab Main window (Console Tab)
        // TODO check that value is 0
        CallScilabBridge::setRootAsParent(getScilabJavaVM(), pUIMENU_FEATURE(sciObj)->hashMapIndex);
        return SET_PROPERTY_SUCCEED;
    }
#endif
}

void EnableRootMenu(char *name, BOOL status)
{
    CallScilabBridge::setRootMenuEnabled(getScilabJavaVM(), name, BOOLtobool(status));
}

void EnableRootSubMenu(char *name, int position, BOOL status)
{
    CallScilabBridge::setRootSubMenuEnabled(getScilabJavaVM(), name, position, BOOLtobool(status));
}

void EnableFigureMenu(int figurenum, char *name, BOOL status)
{
    CallScilabBridge::setFigureMenuEnabled(getScilabJavaVM(), figurenum, name, BOOLtobool(status));
}

void EnableFigureSubMenu(int figurenum, char *name, int position, BOOL status)
{
    CallScilabBridge::setFigureSubMenuEnabled(getScilabJavaVM(), figurenum, name, position, BOOLtobool(status));
}

void DeleteRootMenu(char *name)
{
    CallScilabBridge::removeRootMenu(getScilabJavaVM(), name);
}

void DeleteFigureMenu(int figurenum, char *name)
{
    CallScilabBridge::removeFigureMenu(getScilabJavaVM(), figurenum, name);
}
