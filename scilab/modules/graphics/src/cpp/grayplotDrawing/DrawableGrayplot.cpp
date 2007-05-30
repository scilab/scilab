/*------------------------------------------------------------------------*/
/* file: DrawableGrayplot.cpp                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a grayplot object      */
/*------------------------------------------------------------------------*/

#include "DrawableGrayplot.h"
extern "C"
{
#include "handleDrawing/drawGrayplotEntity.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
void DrawableGrayplot::draw( void )
{
  //drawGrayplotEntity( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableGrayplot::show( void )
{
  draw() ;
}
/*------------------------------------------------------------------------------------------*/

}
