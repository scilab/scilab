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
/*--------------------------------------------------------------------------*/
int gw_graphic_objects(void)
{
    Scierror(999, _("Scilab '%s' module not installed.\n"), "graphic_objects");
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
char *createGraphicObject(int _iType)
{
    return NULL;
}
/*--------------------------------------------------------------------------*/
char const* createDataObject(char const* _sId, int _iType)
{
    return NULL;
}
/*--------------------------------------------------------------------------*/
void deleteGraphicObject(char *_sId)
{
}
/*--------------------------------------------------------------------------*/
void deleteDataObject(char const *_sId)
{
}
/*--------------------------------------------------------------------------*/
void getGraphicObjectProperty(char const* _pstID, const int _iName, enum _ReturnType_ _returnType, void **_pvData)
{
    *_pvData = NULL;
}
/*--------------------------------------------------------------------------*/
void releaseGraphicObjectProperty(const int _iName, void * _pvData, enum _ReturnType_ _returnType, int numElements)
{
}
/*--------------------------------------------------------------------------*/
BOOL setGraphicObjectProperty(char const* _psUID, const int _iPropertyName, void const* _pvValue, enum _ReturnType_ _valueTtype, int numElements)
{
    return FALSE;
}
/*--------------------------------------------------------------------------*/
void setGraphicObjectRelationship(char const* _parentId, char const* _childId)
{
}
/*--------------------------------------------------------------------------*/
