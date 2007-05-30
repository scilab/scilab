/*------------------------------------------------------------------------*/
/* file: DrawableAxes.cpp                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for an axes object         */
/*------------------------------------------------------------------------*/

#include "DrawableAxes.h"
extern "C"
{
#include "handleDrawing/drawAxesEntity.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
void DrawableAxes::draw( void )
{
  //drawAxesEntity( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableAxes::show( void )
{
  draw() ;
}
/*------------------------------------------------------------------------------------------*/

}
