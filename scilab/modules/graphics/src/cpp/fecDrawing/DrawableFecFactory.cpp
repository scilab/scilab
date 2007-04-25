/*------------------------------------------------------------------------*/
/* file: DrawableFecFactory.cpp                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for fec objects. Automaticaly create a fec              */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableFecFactory.h"
#include "DrawableFec.h"

/*------------------------------------------------------------------------------------------*/
DrawableObject * DrawableFecFactory::create( void )
{
  return new DrawableFec( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/
