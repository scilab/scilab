/*------------------------------------------------------------------------*/
/* file: DrawableText.cpp                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a text object          */
/*------------------------------------------------------------------------*/

#include "DrawableText.h"
extern "C"
{
#include "handleDrawing/drawTextEntity.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
void DrawableText::draw( void )
{
  //drawTextEntity( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableText::show( void )
{
  draw() ;
}
/*------------------------------------------------------------------------------------------*/

}
