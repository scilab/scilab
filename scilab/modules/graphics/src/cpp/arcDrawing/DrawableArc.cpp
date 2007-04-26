/*------------------------------------------------------------------------*/
/* file: DrawableArc.cpp                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for an arc object          */
/*------------------------------------------------------------------------*/

#include "DrawableArc.h"
extern "C"
{
#include "handleDrawing/drawArcEntity.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
void DrawableArc::draw( void )
{
  drawArcEntity( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableArc::show( void )
{
  draw() ;
}
/*------------------------------------------------------------------------------------------*/

}
