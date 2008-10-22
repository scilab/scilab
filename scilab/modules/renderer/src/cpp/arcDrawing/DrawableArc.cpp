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
DrawableObject::EDisplayStatus DrawableArc::draw( void )
{
  
  if ( !checkVisibility() )
  {
    return UNCHANGED;
  }
  initializeDrawing() ;
  clip();
  reinitMove();
  drawArc() ;
  unClip();
  endDrawing();
  return SUCCESS;
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableArc::redraw(void)
{
  // force redrawing
  if ( !checkVisibility() )
  {
    return UNCHANGED;
  }
  initializeDrawing() ;
  clip();
  translate();
  redrawArc() ;
  endTranslate();
  unClip();
  endDrawing();
  return SUCCESS;
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableArc::show( void )
{
  if ( !checkVisibility() )
  {
    return UNCHANGED ;
  }
  clip();
  translate();
  showArc();
  endTranslate();
  unClip();
  return SUCCESS;
}
/*---------------------------------------------------------------------------------*/
DrawableArcBridge * DrawableArc::getArcImp( void )
{
  return dynamic_cast<DrawableArcBridge *>(m_pImp);
}
/*---------------------------------------------------------------------------------*/

}
