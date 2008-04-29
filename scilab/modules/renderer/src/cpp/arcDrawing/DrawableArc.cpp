/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the drawing routine for an arc object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableArc.h"

namespace sciGraphics
{


/*---------------------------------------------------------------------------------*/
DrawableArc::DrawableArc( sciPointObj * pObj ) : DrawableClippedObject(pObj)
{
  
}
/*---------------------------------------------------------------------------------*/
DrawableArc::~DrawableArc( void )
{
  
}
/*---------------------------------------------------------------------------------*/
void DrawableArc::draw( void )
{
  
  if ( !checkVisibility() )
  {
    return ;
  }
  initializeDrawing() ;
  clip();
  drawArc() ;
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableArc::redraw(void)
{
  // force redrawing
  draw();
}
/*---------------------------------------------------------------------------------*/
void DrawableArc::show( void )
{
  if ( !checkVisibility() )
  {
    return ;
  }
  clip();
  showArc();
  unClip();
}
/*---------------------------------------------------------------------------------*/
DrawableArcBridge * DrawableArc::getArcImp( void )
{
  return dynamic_cast<DrawableArcBridge *>(m_pImp);
}
/*---------------------------------------------------------------------------------*/

}
