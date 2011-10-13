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

#ifndef __GETGRAPHICOBJECTPROPERTY_H__
#define __GETGRAPHICOBJECTPROPERTY_H__

#include <stdlib.h>

#include "returnType.h"
#include "dynlib_graphic_objects.h"

/*
** Retrieve Graphic object property from Model.
** Specify type expected as parameter for JNI.
*/
GRAPHIC_OBJECTS_IMPEXP void getGraphicObjectProperty(char *_pstID, char *_pstName, enum _ReturnType_ _returnType, void **_pvData);

#endif /* !__GETGRAPHICOBJECTPROPERTY_H__ */
