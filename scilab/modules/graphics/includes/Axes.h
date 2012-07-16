/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010-2012 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*======================================================================================*/
/* Copyrigth INRIA 2006                                                                 */
/* Desc   : Various functions to manipulate SubWindow objects                           */
/*======================================================================================*/

#ifndef __SCI_AXES_H__
#define __SCI_AXES_H__

#include "dynlib_graphics.h"
#include "ObjectStructure.h"


/*-----------------------------------------------------------------------------*/

/* reinit the viewing angles of a subwindow */
GRAPHICS_IMPEXP void initSubWinAngles(char * pSubWinUID);

/* set the data_bounds of the axes to the default value */
GRAPHICS_IMPEXP void initSubWinBounds(char * pSubWinUID);

/* reinit the selected subwindow if the auto_clear property is set to on */
/* return TRUE if the window has been redrawn */
GRAPHICS_IMPEXP BOOL checkRedrawing(void);

/**
 * Convert property name into a sciLegendPlace
 */
GRAPHICS_IMPEXP sciLegendPlace propertyNameToLegendPlace(const char * string);

/*
 * Converts a boolean log flag to the character format
 */
GRAPHICS_IMPEXP char getTextLogFlag(int logFlag);

/*
 * Converts a character log flag to the equivalent boolean
 */
GRAPHICS_IMPEXP int getBooleanLogFlag(char logFlag);

/*-----------------------------------------------------------------------------*/
#endif /*__SCI_AXES_H_*/
