/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: CurrentObjectsManagement.h                                       */
/* desc : Set of functions to set and get current objects                 */
/*------------------------------------------------------------------------*/

#ifndef _GET_CURRENT_OBJECTS_H_
#define _GET_CURRENT_OBJECTS_H_

#include "dynlib_graphics.h"
#include "ObjectStructure.h"
#include "BOOL.h"
/**
 * Returns the pointer to the current selected figure. 
 */
GRAPHICS_IMPEXP sciPointObj * sciGetCurrentFigure( void ) ;

/**
 * To know if a figure is the selected one.
 */
GRAPHICS_IMPEXP BOOL sciIsCurrentFigure(sciPointObj * pFigure);

/**
 * Modify the current figure pointeR.
 */
GRAPHICS_IMPEXP int sciInitCurrentFigure( sciPointObj * mafigure ) ;

/**
 * Modify the current figure if needed.
 */
GRAPHICS_IMPEXP int sciSetCurrentFigure( sciPointObj * mafigure ) ;
/*-----------------------------------------------------------------------------*/
/**
 * Get the current Object
 */
GRAPHICS_IMPEXP sciPointObj * sciGetCurrentObj( void ) ;

/**
 * Modify the current object pointer.
 */
GRAPHICS_IMPEXP void sciSetCurrentObj( sciPointObj * pobj ) ;

/**
 * @return TRUE if the object is the current one, false otherwise
 */
GRAPHICS_IMPEXP BOOL sciIsCurrentObject(sciPointObj * pobj);

/**
 * Return the handle on the current object
 */
GRAPHICS_IMPEXP long sciGetCurrentHandle( void ) ;
/*-----------------------------------------------------------------------------*/
GRAPHICS_IMPEXP sciPointObj * sciGetCurrentSubWin( void ) ;
/*-----------------------------------------------------------------------------*/

#endif /* _GET_CURRENT_OBJECTS_H_ */
