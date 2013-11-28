/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __SETGRAPHICOBJECTPROPERTY_H__
#define __SETGRAPHICOBJECTPROPERTY_H__

#include <stdlib.h>

#include "BOOL.h"

#include "returnType.h"
#include "dynlib_graphic_objects.h"
#include "CurrentObject.h"

GRAPHIC_OBJECTS_IMPEXP void setGraphicObjectRelationship(int iParentId, int iChildId);
GRAPHIC_OBJECTS_IMPEXP BOOL setGraphicObjectProperty(int iUID, const int _iPropertyName, void const* _pvValue, enum _ReturnType_ _valueTtype, int numElements);
GRAPHIC_OBJECTS_IMPEXP BOOL setGraphicObjectPropertyAndNoWarn(int iUID, const int _iPropertyName, void const* _pvValue, enum _ReturnType_ _valueTtype, int numElements);
GRAPHIC_OBJECTS_IMPEXP BOOL setGraphicObjectPropertyAndWarn(int iUID, const int _iPropertyName, void const* _pvValue, enum _ReturnType_ _valueTtype, int numElements, int warnJava);

#endif /* !__SETGRAPHICOBJECTPROPERTY_H__ */
