/*------------------------------------------------------------------------*/
/* file: DrawablePolyline.cpp                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a polyline object      */
/*------------------------------------------------------------------------*/

#include "DrawablePolyline.h"
extern "C"
{
#include "handleDrawing/drawPolylineEntity.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
void DrawablePolyline::draw( void )
{
  drawPolylineEntity( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/

}
