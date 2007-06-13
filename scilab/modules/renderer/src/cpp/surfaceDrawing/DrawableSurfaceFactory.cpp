/*------------------------------------------------------------------------*/
/* file: DrawableSurfaceFactory.cpp                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for surface objects. Automaticaly create a surface      */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableSurfaceFactory.h"
#include "DrawableSurface.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableObject * DrawableSurfaceFactory::create( void )
{
  return new DrawableSurface( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableSurfaceFactory::update( void )
{
  // nothing for now
}
/*------------------------------------------------------------------------------------------*/

}
