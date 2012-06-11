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

#ifndef __FIGURE_LIST_H__
#define __FIGURE_LIST_H__

#include "dynlib_graphic_objects.h"
#include "BOOL.h"

/**
 * @return number of opened windows in Scilab.
 */
GRAPHIC_OBJECTS_IMPEXP int sciGetNbFigure(void);

/**
 * Fill the array Ids with all the figure ids currently used by Scilab.
 * @param ids should be as long as there are figures.
 */
GRAPHIC_OBJECTS_IMPEXP void sciGetFiguresId(int ids[]);

/**
 * @return TRUE if the figure with index id exists
 */
GRAPHIC_OBJECTS_IMPEXP BOOL sciIsExistingFigure(int figNum);

/**
 * Register Scilab View to Graphic Controller
 */
GRAPHIC_OBJECTS_IMPEXP void registerToController(void);

/**
 * Unregister Scilab View to Graphic Controller
 */
GRAPHIC_OBJECTS_IMPEXP void unregisterToController(void);

/**
 * to know if there are some opened graphic windows
 */
GRAPHIC_OBJECTS_IMPEXP BOOL sciHasFigures(void);

/**
 * returns the UID of window i
 * or NULL if this window does not exists
 */
GRAPHIC_OBJECTS_IMPEXP char const* getFigureFromIndex(int figNum);

#endif /* !__FIGURE_LIST_H__ */
