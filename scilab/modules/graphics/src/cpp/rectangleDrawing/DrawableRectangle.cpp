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

/*------------------------------------------------------------------------------------------*/
void DrawableRectangle::draw( void )
{
  drawRectangleEntity( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/
