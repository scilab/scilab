/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 * Copyright (C) 2008 - Yung-Jang Lee
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

#include "InitUIMenu.hxx"
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
#include "getConsoleIdentifier.h"
#include "graphicObjectProperties.h"
#include "CurrentFigure.h"
#include "BuildObjects.h"
#include "configvariable_interface.h"
#include "createGraphicObject.h"
}

using namespace org_scilab_modules_gui_bridge;

int setMenuParent(int iObjUID, void* pvData, int valueType, int nbRow, int nbCol)
{
    int iCurrentFigure = 0;
    int parentType = -1;
    int *piParentType = &parentType;
    int iParentUID = 0;

    double *value = NULL;

    /* Special case to set current figure for parent */
    if (pvData == NULL)
    {
        // Set the parent property
        iCurrentFigure = getCurrentFigure();
        if (iCurrentFigure == 0)
        {
            iCurrentFigure = createNewFigureWithAxes();
        }
        setGraphicObjectRelationship(iCurrentFigure, iObjUID);
        return SET_PROPERTY_SUCCEED;
    }

    if (nbRow * nbCol != 1)
    {
        // Parent must be a single value
        return SET_PROPERTY_ERROR;
    }

    /* Check parent type */
    if (getScilabMode() == SCILAB_STD)
    {
        /* Figure, uimenu or Console can be the parent */
        if ((valueType != sci_handles) && (valueType != sci_matrix))
        {
            Scierror(999, const_cast < char *>(_("%s: Wrong type for parent: A handle or 0 expected.\n")), "SetMenuParent");

            return 0;
        }
    }
    else
    {
        /* Figure, uimenu can be the parent */
        if (valueType == sci_matrix)
        {
            Scierror(999, const_cast < char *>(_("%s: can not add a menu into the console in this mode.\n")), "SetMenuParent");
            return 0;
        }
        else if (valueType != sci_handles)
        {
            Scierror(999, const_cast < char *>(_("%s: Wrong type for parent: A handle expected.\n")), "SetMenuParent");
            return 0;
        }
    }

    if (valueType == sci_handles)
    {
        iParentUID = getObjectFromHandle((long)((long long*)pvData)[0]);
        if (iParentUID)
        {
            getGraphicObjectProperty(iParentUID, __GO_TYPE__, jni_int, (void **)&piParentType);
            if (parentType == __GO_FIGURE__ || parentType == __GO_UIMENU__
                    || parentType == __GO_UICONTEXTMENU__)
            {
                setGraphicObjectRelationship(iParentUID, iObjUID);
            }
            else
            {
                Scierror(999, const_cast < char *>(_("%s: Wrong type for parent: Figure or uimenu handle expected.\n")), "SetMenuParent");
                return SET_PROPERTY_ERROR;
            }
        }
        else
        {
            Scierror(999, const_cast < char *>(_("%s: Wrong type for parent: Figure or uimenu handle expected.\n")), "SetMenuParent");

            return SET_PROPERTY_ERROR;
        }
    }

    if (valueType == sci_matrix)
    {
        // The parent is Scilab Main window (Console Tab)
        value = (double*)pvData;
        if (value[0] != 0)
        {
            Scierror(999, const_cast < char *>(_("%s: Wrong value for parent: 0 expected.\n")), "SetMenuParent");

            return SET_PROPERTY_ERROR;
        }
        setGraphicObjectRelationship(getConsoleIdentifier(), iObjUID);
    }

    return SET_PROPERTY_SUCCEED;
}

void EnableMenu(int iParentId, char *name, BOOL status)
{
    CallScilabBridge::setMenuEnabled(getScilabJavaVM(), iParentId, name, BOOLtobool(status));
}

void EnableSubMenu(int iParentId, char *name, int position, BOOL status)
{
    CallScilabBridge::setSubMenuEnabled(getScilabJavaVM(), iParentId, name, position, BOOLtobool(status));
}

void DeleteMenuWithName(int iParentId, char *name)
{
    CallScilabBridge::removeMenu(getScilabJavaVM(), iParentId, name);
}
