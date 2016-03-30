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

#ifndef __CURRENT_FIGURE_H__
#define __CURRENT_FIGURE_H__

#include "dynlib_graphic_objects.h"
/**
 * Returns UID of the current selected figure.
 */
GRAPHIC_OBJECTS_IMPEXP int getCurrentFigure(void);

/**
 * Modify the current figure if needed.
 */
GRAPHIC_OBJECTS_IMPEXP void setCurrentFigure(int iUID);

/**
 * To know if a figure is the selected one.
 */
GRAPHIC_OBJECTS_IMPEXP BOOL isCurrentFigure(int iUID);

#endif /* !__CURRENT_FIGURE_H__ */
