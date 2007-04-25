/*------------------------------------------------------------------------*/
/* file: DrawableLegendFactory.cpp                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for legend objects. Automaticaly create a legend        */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableLegendFactory.h"
#include "DrawableLegend.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableObject * DrawableLegendFactory::create( void )
{
  return new DrawableLegend( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/

}
