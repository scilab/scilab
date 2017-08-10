/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
