/*------------------------------------------------------------------------*/
/* file: DrawableTextFactory.cpp                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for text objects. Automaticaly create a text            */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableTextFactory.h"
#include "DrawableText.h"
#include "DrawableTextBridgeFactory.hxx"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableObject * DrawableTextFactory::create( void )
{
  DrawableText * newText = new DrawableText( m_pDrawed ) ;
  DrawableTextBridgeFactory fact;
  fact.setDrawedText(newText);
  newText->setDrawableImp(fact.create());
  return newText;
}
/*------------------------------------------------------------------------------------------*/
void DrawableTextFactory::update( void )
{
  // nothing for now
}
/*------------------------------------------------------------------------------------------*/

}
