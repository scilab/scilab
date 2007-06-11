/*------------------------------------------------------------------------*/
/* file: DrawableTextFactory.cpp                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for text objects. Automaticaly create a text            */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableTextFactory.h"
#include "DrawableText.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableObject * DrawableTextFactory::create( void )
{
  return new DrawableText( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableTextFactory::update( void )
{
  // nothing for now
}
/*------------------------------------------------------------------------------------------*/

}
