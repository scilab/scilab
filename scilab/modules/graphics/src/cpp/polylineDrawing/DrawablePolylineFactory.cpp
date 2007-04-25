/*------------------------------------------------------------------------*/
/* file: DrawablePolylineFactory.cpp                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for polyline objects. Automaticaly create a polyline    */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawablePolylineFactory.h"
#include "DrawablePolyline.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableObject * DrawablePolylineFactory::create( void )
{
  return new DrawablePolyline( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/

}
