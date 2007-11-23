/*------------------------------------------------------------------------*/
/* file: DrawableFigureFactory.cpp                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for figure objects. Automaticaly create a figure        */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableFigureFactory.h"
#include "ConcreteDrawableFigure.h"
#include "DrawableFigureBridge.h"
#include "DrawableFigureBridgeFactory.h"
#include "GraphicSynchronization/GraphicSynchronizerFactory.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject * DrawableFigureFactory::create( void )
{
  DrawableFigure * newFig = new ConcreteDrawableFigure( m_pDrawed ) ;

  // create bridge
  DrawableFigureBridgeFactory imp ;
  imp.setDrawedFigure( newFig ) ;

  newFig->setDrawableImp( imp.create() ) ;

  // create synchronizer
  GraphicSynchronizerFactory fact;
  newFig->setSynchronizer(fact.createLocalSynchronizer());

  newFig->openRenderingCanvas();

  return newFig ;

}
/*---------------------------------------------------------------------------------*/
void DrawableFigureFactory::update( void )
{
  // nothing for now
}
/*---------------------------------------------------------------------------------*/

}
