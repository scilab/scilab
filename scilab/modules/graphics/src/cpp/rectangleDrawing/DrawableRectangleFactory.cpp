/*------------------------------------------------------------------------*/
/* file: DrawableRectangleFactory.cpp                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for rectangle objects. Automaticaly create a rectangle  */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableRectangleFactory.h"
#include "DrawableRectangle.h"

/*------------------------------------------------------------------------------------------*/
DrawableObject * DrawableRectangleFactory::create( void )
{
  return new DrawableRectangle( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/
