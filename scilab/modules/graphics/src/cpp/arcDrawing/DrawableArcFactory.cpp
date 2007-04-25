/*------------------------------------------------------------------------*/
/* file: DrawableArcFactory.cpp                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for arc objects. Automaticaly create an arc             */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableArcFactory.h"
#include "DrawableArc.h"

/*------------------------------------------------------------------------------------------*/
DrawableObject * DrawableArcFactory::create( void )
{
  return new DrawableArc( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/
