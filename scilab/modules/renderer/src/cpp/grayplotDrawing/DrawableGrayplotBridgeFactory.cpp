/*------------------------------------------------------------------------*/
/* file: DrawableGrayplotBridgeFactory.hxx                                */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableGrayplotBridge                              */
/*------------------------------------------------------------------------*/

#include "DrawableGrayplotBridgeFactory.hxx"
#include "DrawableGrayplotJoGL.hxx"
#include "DrawableGrayplotJavaMapper.hxx"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableGrayplotBridge * DrawableGrayplotBridgeFactory::create( void )
{
  DrawableGrayplotJoGL * imp = new DrawableGrayplotJoGL( m_pDrawable ) ;
  imp->setJavaMapper(new DrawableGrayplotJavaMapper()) ;
  return imp ;
}
/*---------------------------------------------------------------------------------*/
void DrawableGrayplotBridgeFactory::setDrawedGrayplot( DrawableGrayplot * poly )
{
  m_pDrawable = poly ;
}
/*---------------------------------------------------------------------------------*/

}
