/*------------------------------------------------------------------------*/
/* file: DrawableLabelFactory.cpp                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for label objects. Automaticaly create a label          */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableLabelFactory.h"
#include "DrawableLabel.h"

/*------------------------------------------------------------------------------------------*/
DrawableObject * DrawableLabelFactory::create( void )
{
  return new DrawableLabel( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/
