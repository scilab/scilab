/*------------------------------------------------------------------------*/
/* file: DrawableRectangle.cpp                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a rectangle object     */
/*------------------------------------------------------------------------*/



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
