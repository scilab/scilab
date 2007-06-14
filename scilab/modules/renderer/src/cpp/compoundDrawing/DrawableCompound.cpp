/*------------------------------------------------------------------------*/
/* file: DrawableCompound.cpp                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a compound object      */
/*------------------------------------------------------------------------*/


#include "DrawableCompound.h"
extern "C"
{
#include "drawCompoundEntity.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
void DrawableCompound::draw( void )
{
  drawCompoundEntity( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableCompound::show( void )
{
  draw() ;
}
/*------------------------------------------------------------------------------------------*/

}
