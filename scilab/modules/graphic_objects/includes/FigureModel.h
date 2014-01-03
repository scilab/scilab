/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __FIGURE_MODEL_H__
#define __FIGURE_MODEL_H__

#include "dynlib_graphic_objects.h"
#include "BOOL.h"

GRAPHIC_OBJECTS_IMPEXP int getFigureModel(void);

GRAPHIC_OBJECTS_IMPEXP void setFigureModel(int iUID);

GRAPHIC_OBJECTS_IMPEXP BOOL isFigureModel(int iUID);

#endif /* !__FIGURE_MODEL_H__ */
