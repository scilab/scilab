/*------------------------------------------------------------------------*/
/* file: DrawableRectangle.cpp                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a rectangle object     */
/*------------------------------------------------------------------------*/

#include "DrawableRectangle.h"
extern "C"
{
#include "handleDrawing/drawRectangleEntity.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
DrawableRectangle::~DrawableRectangle( void )
{

}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangle::draw( void )
{
  if ( !checkVisibility() ) { return ; }
  drawRectangle() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangle::show( void )
{
  draw() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangle::drawRectangle( void )
{
  getRectangleImp()->drawRectangle() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangle::getCornersCoordinates( double corner1[3], double corner2[3], double corner3[3], double corner4[3] )
{
  sciRectangle * ppRect = pRECTANGLE_FEATURE(m_pDrawed) ;
  corner1[0] = ppRect->x ;
  corner1[1] = ppRect->y ;
  corner1[2] = ppRect->z ;

  corner2[0] = ppRect->x ;
  corner2[1] = ppRect->y + ppRect->height ;
  corner2[2] = ppRect->z ;

  corner3[0] = ppRect->x + ppRect->width  ;
  corner3[1] = ppRect->y + ppRect->height ;
  corner3[2] = ppRect->z ;

  corner4[0] = ppRect->x + ppRect->height ;
  corner4[1] = ppRect->y ;
  corner4[2] = ppRect->z ;

}
/*------------------------------------------------------------------------------------------*/
}
