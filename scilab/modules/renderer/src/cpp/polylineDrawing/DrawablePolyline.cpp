/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the drawing routine for a polyline object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawablePolyline.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawablePolylineBridge * DrawablePolyline::getPolylineImp( void )
{
  return dynamic_cast<DrawablePolylineBridge *>(m_pImp) ;
}
/*---------------------------------------------------------------------------------*/
void DrawablePolyline::draw( void )
{
  if ( !checkVisibility() )
  {
    return ;
  }
  initializeDrawing() ;
  clip();
  drawPolyline() ;
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawablePolyline::show( void )
{
  if ( !checkVisibility() )
  {
    return ;
  }
  initializeDrawing();
  clip();
  showPolyline();
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/

}
