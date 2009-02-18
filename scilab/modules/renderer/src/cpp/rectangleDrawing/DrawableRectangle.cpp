/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the drawing routine for a rectangle object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */



#include "DrawableRectangle.h"

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
DrawableRectangle::DrawableRectangle( sciPointObj * pObj ) : DrawableClippedObject( pObj )
{

}
/*---------------------------------------------------------------------------------*/
DrawableRectangle::~DrawableRectangle( void )
{

}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableRectangle::draw( void )
{
  
  if ( !checkVisibility() )
  {
    return UNCHANGED;
  }
  initializeDrawing() ;
  clip();
  reinitMove();
  drawRectangle() ;
  unClip();
  endDrawing();
  return SUCCESS;
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableRectangle::show( void )
{
  if ( !checkVisibility() )
  {
    return UNCHANGED;
  }
  clip();
  translate();
  showRectangle();
  endTranslate();
  unClip();
  return SUCCESS;
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableRectangle::redraw(void)
{
  if ( !checkVisibility() )
  {
    return UNCHANGED;
  }
  initializeDrawing();
  clip();
  translate();
  redrawRectangle();
  endTranslate();
  unClip();
  endDrawing();
  return SUCCESS;
}
/*---------------------------------------------------------------------------------*/
DrawableRectangleBridge * DrawableRectangle::getRectangleImp( void )
{
  return dynamic_cast<DrawableRectangleBridge *>(m_pImp) ;
}
/*---------------------------------------------------------------------------------*/

}
