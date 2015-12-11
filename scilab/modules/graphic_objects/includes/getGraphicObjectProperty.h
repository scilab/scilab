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

#ifndef __GETGRAPHICOBJECTPROPERTY_H__
#define __GETGRAPHICOBJECTPROPERTY_H__

#include <stdlib.h>

#include "returnType.h"
#include "dynlib_graphic_objects.h"
#include "CurrentObject.h"

/*
** Retrieve Graphic object property from Model.
** Specify type expected as parameter for JNI.
*/
GRAPHIC_OBJECTS_IMPEXP void getGraphicObjectProperty(int iID, const int _iName, enum _ReturnType_ _returnType, void **_pvData);

/**
 * @brief release a Graphic object property from the Model.
 *
 * Data are free/delete if they are passed by copy. Passed by copy properties are :
 *  1. out of DataModel properties
 *  2. with datatype passed as pointer or pointer of pointer
 *
 * @param _pstName property name
 * @param _pvData the data to release
 * @param _returnType the property type descriptor
 * @param numElements size of the _pvData argument
 */
GRAPHIC_OBJECTS_IMPEXP void releaseGraphicObjectProperty(const int _iName, void * _pvData, enum _ReturnType_ _returnType, int numElements);

#endif /* !__GETGRAPHICOBJECTPROPERTY_H__ */
