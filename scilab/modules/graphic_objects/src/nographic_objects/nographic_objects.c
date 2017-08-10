/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#include "nographic_objects.h"
#include "Scierror.h"
#include "localization.h"
#include "FigureList.h"
#include "returnType.h"
#include "createGraphicObject.h"
#include "deleteGraphicObject.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#ifndef _MSC_VER
#include "configvariable_interface.h"
#endif
/*--------------------------------------------------------------------------*/
int gw_graphic_objects(void)
{
#ifndef _MSC_VER
    if (getScilabMode() == SCILAB_NWNI)
    {
        Scierror(999, _("Scilab '%s' module disabled in -nogui or -nwni mode.\n"), "graphic_objects");
    }
    else
#endif
    {
        Scierror(999, _("Scilab '%s' module not installed.\n"), "graphic_objects");
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
BOOL sciHasFigures(void)
{
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL sciIsExistingFigure(int figNum)
{
    return FALSE;
}
/*--------------------------------------------------------------------------*/
int createGraphicObject(int _iType)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
int createDataObject(int iUID, int _iType)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
void deleteGraphicObject(int iUID)
{
}
/*--------------------------------------------------------------------------*/
void deleteDataObject(int iUID)
{
}
/*--------------------------------------------------------------------------*/
void getGraphicObjectProperty(int iUID, const int _iName, enum _ReturnType_ _returnType, void **_pvData)
{
    *_pvData = NULL;
}
/*--------------------------------------------------------------------------*/
void releaseGraphicObjectProperty(const int _iName, void * _pvData, enum _ReturnType_ _returnType, int numElements)
{
}
/*--------------------------------------------------------------------------*/
BOOL setGraphicObjectProperty(int iUID, const int _iPropertyName, void const* _pvValue, enum _ReturnType_ _valueTtype, int numElements)
{
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL setGraphicObjectPropertyAndNoWarn(int iUID, const int _iPropertyName, void const* _pvValue, enum _ReturnType_ _valueTtype, int numElements)
{
    return FALSE;
}
/*--------------------------------------------------------------------------*/
void setGraphicObjectRelationship(int iUID, int iUID2)
{
}
/*--------------------------------------------------------------------------*/
long getHandle(int UID)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
int createFigure(int iDockable, int iMenubarType, int iToolbarType, int iDefaultAxes, int iVisible)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
int createNewFigureWithAxes(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
int getObjectFromHandle(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
int getValidDefaultFigureId(void)
{
    return 0;
}

int getCurrentFigure(void)
{
    return 0;
}

int createSubWin(int iParent)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
