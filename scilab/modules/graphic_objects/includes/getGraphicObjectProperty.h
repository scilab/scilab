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
