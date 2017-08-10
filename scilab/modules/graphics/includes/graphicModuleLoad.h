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
/* file: graphicModuleClose.h                                             */
/* desc : function to be called each time the graphic module is unloaded  */
/*------------------------------------------------------------------------*/

#ifndef _GRAPHIC_MODULE_CLOSE_H_
#define _GRAPHIC_MODULE_CLOSE_H_

#include "dynlib_graphics.h"

/**
 * Routine creating all needed objects for the module
 */
GRAPHICS_IMPEXP void loadGraphicModule(void);

/**
 * Closing function freeing all memory used by the graphic module
 */
GRAPHICS_IMPEXP void closeGraphicModule(void);

#endif /* _GRAPHIC_MODULE_CLOSE_H_ */
