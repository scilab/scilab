/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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

#ifndef __FIGURE_MODEL_H__
#define __FIGURE_MODEL_H__

#include "dynlib_graphic_objects.h"
#include "BOOL.h"

GRAPHIC_OBJECTS_IMPEXP int getFigureModel(void);

GRAPHIC_OBJECTS_IMPEXP void setFigureModel(int iUID);

GRAPHIC_OBJECTS_IMPEXP BOOL isFigureModel(int iUID);

#endif /* !__FIGURE_MODEL_H__ */
