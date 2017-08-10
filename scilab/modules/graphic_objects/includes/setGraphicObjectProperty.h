/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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
