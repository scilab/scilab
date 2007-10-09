/*------------------------------------------------------------------------*/
/* file: DrawableTextFactory.cpp                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for text objects. Automaticaly create a text            */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableTextFactory.h"
#include "ConcreteDrawableText.hxx"
#include "DrawableTextBridgeFactory.hxx"
#include "StandardTextDrawerJoGL.hxx"
#include "FilledTextDrawerJoGL.hxx"
#include "getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableObject * DrawableTextFactory::create( void )
{
  ConcreteDrawableText * newText = new ConcreteDrawableText( m_pDrawed ) ;
  DrawableTextBridgeFactory fact;
  fact.setDrawedText(newText);
  newText->setDrawableImp(fact.create());
  setStrategies(newText);

  return newText;
}
/*------------------------------------------------------------------------------------------*/
void DrawableTextFactory::update( void )
{
  setStrategies(dynamic_cast<ConcreteDrawableText *>(getTextDrawer(m_pDrawed)));
}
/*------------------------------------------------------------------------------------------*/
void DrawableTextFactory::setStrategies(ConcreteDrawableText * text)
{
  text->removeTextDrawingStrategy();
  text->removeBoxDrawingStrategies();

  sciPointObj * pText = text->getDrawedObject();

  if ( sciGetAutoSize(pText) )
  {
    text->setTextDrawingStrategy(new StandardTextDrawerJoGL(text));
  }
  else
  {
    text->setTextDrawingStrategy(new FilledTextDrawerJoGL(text));
  }


}
/*------------------------------------------------------------------------------------------*/

}
