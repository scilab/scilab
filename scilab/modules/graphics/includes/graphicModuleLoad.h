/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
 * Routine creating all needed objetcs for the module
 */
GRAPHICS_IMPEXP void loadGraphicModule( void ) ;

/**
 * Closing function freeing all memory used by the graphic module
 */
GRAPHICS_IMPEXP void closeGraphicModule( void ) ;

#endif /* _GRAPHIC_MODULE_CLOSE_H_ */
