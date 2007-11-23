/*------------------------------------------------------------------------*/
/* file: DrawableCompoundFactory.cpp                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for compound objects. Automaticaly create a compound    */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableCompoundFactory.h"
#include "DrawableCompound.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject * DrawableCompoundFactory::create( void )
{
  return new DrawableCompound( m_pDrawed ) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableCompoundFactory::update( void )
{
  // nothing for now
}
/*---------------------------------------------------------------------------------*/

}
