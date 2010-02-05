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

/*======================================================================================*/
/* Copyrigth INRIA 2006                                                                 */
/* Desc   : Various functions to manipulate SubWindow objects                           */
/*======================================================================================*/

#ifndef __SCI_AXES_H__
#define __SCI_AXES_H__

#include "dynlib_graphics.h"
#include "ObjectStructure.h"


/*-----------------------------------------------------------------------------*/

/* clear a subwindow from all of its children */
GRAPHICS_IMPEXP void clearSubWin( sciPointObj * pSubWin ) ;

/* reinit a subwindow (but don't change position) */
GRAPHICS_IMPEXP void reinitSubWin( sciPointObj * pSubWin ) ;

/* reinit the viewing angles of a subwindow */
GRAPHICS_IMPEXP void initSubWinAngles( sciPointObj * pSubWin ) ;

/* set the size and position of the subwindow to the default */
GRAPHICS_IMPEXP void initSubWinSize( sciPointObj * pSubWin ) ;

/* set the data_bounds of the axes to the default value */
GRAPHICS_IMPEXP void initSubWinBounds( sciPointObj * pSubWin ) ;

/* reinit the selected subwindow if the auto_clear property is set to on */
/* return TRUE if the window has been redrawn */
GRAPHICS_IMPEXP BOOL checkRedrawing( void ) ;

/**
 * Find which subwindow is selected by clincking at pixel coordinates (xCoord, yCoord).
 * @param pFigure index of parent figure in which the subwindow will be selected
 * @return NULL if no subwindow could be selected, the seleceted subwindow otherwise
 */
GRAPHICS_IMPEXP sciPointObj * getClickedSubwin(sciPointObj * pFigure, int xCoord, int yCoord);

/**
 * Convert property name into a sciLegendPlace
 */
GRAPHICS_IMPEXP sciLegendPlace propertyNameToLegendPlace(const char * string);

/*-----------------------------------------------------------------------------*/
#endif /*__SCI_AXES_H_*/
