/*------------------------------------------------------------------------*/
/* file: DrawableFec.cpp                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a fec object        */
/*------------------------------------------------------------------------*/

#include "DrawableFec.h"
extern "C"
{
#include "drawFecEntity.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
void DrawableFec::draw( void )
{
  //drawFecEntity( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFec::show( void )
{
  draw() ;
}
/*------------------------------------------------------------------------------------------*/

}
