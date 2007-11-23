/*------------------------------------------------------------------------*/
/* file: DrawableTextBridgeFactory.cpp                                    */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableTextBridge                                  */
/*------------------------------------------------------------------------*/

#include "DrawableTextBridgeFactory.hxx"
#include "DrawableTextJoGL.hxx"
#include "DrawableTextJavaMapper.hxx"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableTextBridge * DrawableTextBridgeFactory::create( void )
{
  DrawableTextJoGL * imp = new DrawableTextJoGL( m_pDrawable ) ;
  imp->setJavaMapper(new DrawableTextJavaMapper()) ;
  return imp ;
}
/*---------------------------------------------------------------------------------*/
void DrawableTextBridgeFactory::setDrawedText( DrawableText* poly )
{
  m_pDrawable = poly ;
}
/*---------------------------------------------------------------------------------*/

}
