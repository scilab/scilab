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

#ifndef __DELETEGRAPHICOBJECT_H__
#define __DELETEGRAPHICOBJECT_H__

#include "dynlib_graphic_objects.h"

/*
** Ask Controller to delete a graphic object
** referenced by the given _sId.
*/
GRAPHIC_OBJECTS_IMPEXP void deleteGraphicObject(char *_sId);

/*
** Ask Controller to delete a data object
** referenced by the given _sId.
*/
GRAPHIC_OBJECTS_IMPEXP void deleteDataObject(char const *_sId);

#endif /* !__DELETEGRAPHICOBJECT_H__ */
