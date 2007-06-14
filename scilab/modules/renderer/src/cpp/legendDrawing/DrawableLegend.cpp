/*------------------------------------------------------------------------*/
/* file: DrawableLegend.cpp                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a legend object        */
/*------------------------------------------------------------------------*/

#include "DrawableLegend.h"
extern "C"
{
#include "drawLegendEntity.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
void DrawableLegend::draw( void )
{
  //drawLegendEntity( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableLegend::show( void )
{
  draw() ;
}
/*------------------------------------------------------------------------------------------*/


}
