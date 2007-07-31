/*------------------------------------------------------------------------*/
/* file: DrawableFigureImpFactory.cpp                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableFigureImp                                   */
/*------------------------------------------------------------------------*/

#include "DrawableFigureBridgeFactory.h"
#include "DrawableFigureJoGL.h"


namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableFigureBridge * DrawableFigureBridgeFactory::create( void )
{
  return new DrawableFigureJoGL( m_pDrawable ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureBridgeFactory::setDrawedFigure( DrawableFigure * fig )
{
  m_pDrawable = fig ;
}
/*------------------------------------------------------------------------------------------*/

}
