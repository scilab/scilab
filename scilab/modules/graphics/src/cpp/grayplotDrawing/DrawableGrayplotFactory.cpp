/*------------------------------------------------------------------------*/
/* file: DrawableGRayplotFactory.cpp                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for grayplot objects. Automaticaly create a grayplot    */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableGrayplotFactory.h"
#include "DrawableGrayplot.h"

/*------------------------------------------------------------------------------------------*/
DrawableObject * DrawableGrayplotFactory::create( void )
{
  return new DrawableGrayplot( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/
