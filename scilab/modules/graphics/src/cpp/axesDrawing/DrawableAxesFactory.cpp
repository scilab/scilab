/*------------------------------------------------------------------------*/
/* file: DrawableAxesFactory.cpp                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for axes objects. Automaticaly create an axes           */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableAxesFactory.h"
#include "DrawableAxes.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableObject * DrawableAxesFactory::create( void )
{
  return new DrawableAxes( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableAxesFactory::update( void )
{
 // nothing for now
}
/*------------------------------------------------------------------------------------------*/
}
