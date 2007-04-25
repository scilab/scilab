/*------------------------------------------------------------------------*/
/* file: DrawableSurface.cpp                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a surface object       */
/*------------------------------------------------------------------------*/


#include "DrawableSurface.h"
extern "C"
{
#include "handleDrawing/drawSurfaceEntity.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
void DrawableSurface::draw( void )
{
  drawSurfaceEntity( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/

}
