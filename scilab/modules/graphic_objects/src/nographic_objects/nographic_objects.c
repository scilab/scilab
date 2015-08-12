/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
#include "scilabmode.h"
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
void setGraphicObjectRelationship(int iUID, int iUID2)
{
}
/*--------------------------------------------------------------------------*/
