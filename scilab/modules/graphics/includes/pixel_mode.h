/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: pixel_mode.h                                                     */
/* desc : make the correspondence between pixel mode name in scilab       */
/*        and the corresponding index                                     */
/*------------------------------------------------------------------------*/

#ifndef _PIXEL_MODE_H_
#define _PIXEL_MODE_H_

#include "dynlib_graphics.h"

/**
 * return the number of different pixel drawing modes.
 */
GRAPHICS_IMPEXP int getNbPixelModes(void);

/**
 * Get the name of pixel mode number index.
 */
GRAPHICS_IMPEXP char * getPixelMode(int ind);

/**
 * Look for a pixel mode of name modeName and return its index.
 * If none has been found, return -1. Another negative value is returned
 * if an error occurred.
 */
GRAPHICS_IMPEXP int getPixelModeIndex(const char * modeName);

#endif
