/*------------------------------------------------------------------------*/
/* file: DrawableSubwinFactory.cpp                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for subwin objects. Automaticaly create a subwin        */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableSubwinFactory.h"
#include "DrawableSubwin.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableObject * DrawableSubwinFactory::create( void )
{
  return new DrawableSubwin( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableSubwinFactory::update( void )
{
  // nothing for now
}
/*------------------------------------------------------------------------------------------*/

}
