/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __AXES_MODEL_H__
#define __AXES_MODEL_H__

#include "dynlib_graphic_objects.h"
#include "BOOL.h"

GRAPHIC_OBJECTS_IMPEXP char const* getAxesModel(void);

GRAPHIC_OBJECTS_IMPEXP void setAxesModel(char const* UID);

GRAPHIC_OBJECTS_IMPEXP BOOL isAxesModel(char const* UID);

#endif /* !__AXES_MODEL_H__ */
