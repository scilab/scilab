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
#include <iostream>

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
void DrawableArc::parentSubwinChanged( void )
{
  m_bNeedRedraw = true;
}
/*---------------------------------------------------------------------------------*/
void DrawableArc::draw( void )
{
  initializeDrawing() ;
  if ( !checkVisibility() )
  {
    endDrawing();
    return ;
  }
  clip();
  drawArc() ;
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableArc::show( void )
{
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
