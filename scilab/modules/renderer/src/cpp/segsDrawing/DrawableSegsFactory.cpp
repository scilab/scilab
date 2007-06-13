/*------------------------------------------------------------------------*/
/* file: DrawableSegsFactory.cpp                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for segs objects. Automaticaly create a segs            */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableSegsFactory.h"
#include "DrawableSegs.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableObject * DrawableSegsFactory::create( void )
{
  return new DrawableSegs( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableSegsFactory::update( void )
{
  // nothing for now
}
/*------------------------------------------------------------------------------------------*/

}
