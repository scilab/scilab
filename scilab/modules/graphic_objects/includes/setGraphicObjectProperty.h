/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SETGRAPHICOBJECTPROPERTY_H__
#define __SETGRAPHICOBJECTPROPERTY_H__

#include "BOOL.h"

#include "returnType.h"
#include "dynlib_graphic_objects.h"

GRAPHIC_OBJECTS_IMPEXP BOOL setGraphicObjectProperty(char *_psUID, char *_psPropertyName, void *_pvValue, enum _ReturnType_ _valueTtype, int numElements);
GRAPHIC_OBJECTS_IMPEXP BOOL setGraphicObjectStringVectorProperty(char *_pstID, char *_pstName, void *_pvValue, int numElements);
GRAPHIC_OBJECTS_IMPEXP BOOL setGraphicObjectDoubleProperty(char *_pstID, char *_pstName, double value);
GRAPHIC_OBJECTS_IMPEXP BOOL setGraphicObjectDoubleVectorProperty(char *_pstID, char *_pstName, void *_pvValue, int numElements);
GRAPHIC_OBJECTS_IMPEXP BOOL setGraphicObjectIntegerVectorProperty(char *_pstID, char *_pstName, void *_pvValue, int numElements);
GRAPHIC_OBJECTS_IMPEXP BOOL setGraphicObjectBooleanVectorProperty(char *_pstID, char *_pstName, void *_pvValue, int numElements);

#endif /* !__SETGRAPHICOBJECTPROPERTY_H__ */
