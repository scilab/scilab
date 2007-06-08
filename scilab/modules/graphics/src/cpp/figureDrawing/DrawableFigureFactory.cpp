/*------------------------------------------------------------------------*/
/* file: DrawableFigureFactory.cpp                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for figure objects. Automaticaly create a figure        */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableFigureFactory.h"
#include "ConcreteDrawableFigure.h"
#include "DrawableFigureImp.h"
#include "DrawableFigureImpFactory.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableObject * DrawableFigureFactory::create( void )
{
  DrawableFigure * newFig = new ConcreteDrawableFigure( m_pDrawed ) ;
  DrawableFigureImpFactory imp ;
  imp.setDrawedFigure( newFig ) ;
  newFig->setDrawableImp( imp.create() ) ;

  return newFig ;

}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureFactory::update( void )
{
  // nothing for now
}
/*------------------------------------------------------------------------------------------*/

}
