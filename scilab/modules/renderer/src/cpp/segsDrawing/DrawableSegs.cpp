/*------------------------------------------------------------------------*/
/* file: DrawableSegs.cpp                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a segs object          */
/*------------------------------------------------------------------------*/

#include "DrawableSegs.h"
extern "C"
{
#include "../../../graphics/src/c/handleDrawing/drawSegsEntity.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
void DrawableSegs::draw( void )
{
  //drawSegsEntity( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableSegs::show( void )
{
  draw() ;
}
/*------------------------------------------------------------------------------------------*/

}
