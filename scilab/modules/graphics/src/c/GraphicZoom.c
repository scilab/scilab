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
/* file: GraphicZoom.c                                                    */
/* desc : Contains functions to Zoom in the graphic window                */
/*------------------------------------------------------------------------*/

#include "GraphicZoom.h"

/**
 * To know if the zoom is activated
 */
static BOOL GlobalFlag_Zoom3dOn = FALSE ;
/*-------------------------------------------------------------------------------------*/
BOOL isZoom3dOn( void )
{
  return GlobalFlag_Zoom3dOn ;
}
/*-------------------------------------------------------------------------------------*/
void setZoom3dStatus( BOOL status )
{
  GlobalFlag_Zoom3dOn = status ;
}
/*-------------------------------------------------------------------------------------*/
