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

using namespace std;

/*---------------------------------------------------------------------------------*/
DrawableRectangle::~DrawableRectangle( void )
{

}
/*---------------------------------------------------------------------------------*/
void DrawableRectangle::draw( void )
{

  initializeDrawing() ;
  if ( !checkVisibility() )
  {
    endDrawing();
    return ;
  }
  clip();
  drawRectangle() ;
  unClip();
  endDrawing();

}
/*---------------------------------------------------------------------------------*/
void DrawableRectangle::show( void )
{
  initializeDrawing();
  if ( !checkVisibility() )
  {
    endDrawing();
    return ;
  }
  clip();
  getRectangleImp()->show() ;
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableRectangle::redraw(void)
{
  initializeDrawing();
  if ( !checkVisibility() )
  {
    endDrawing();
    return ;
  }
  clip();
  getRectangleImp()->redrawRectangle();
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
DrawableRectangleBridge * DrawableRectangle::getRectangleImp( void )
{
  return dynamic_cast<DrawableRectangleBridge *>(m_pImp) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableRectangle::drawRectangle( void )
{
  getRectangleImp()->drawRectangle() ;
}
/*---------------------------------------------------------------------------------*/

}
