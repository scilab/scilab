/*------------------------------------------------------------------------*/
/* file: DrawableFigureImpFactory.cpp                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableFigureImp                                   */
/*------------------------------------------------------------------------*/

#include "DrawableFigureImpFactory.h"
#include "DrawableFigureJoGL.h"

/*------------------------------------------------------------------------------------------*/
DrawableFigureImp * DrawableFigureImpFactory::create( void )
{
  return new DrawableFigureJoGL( m_pDrawable ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureImpFactory::setDrawedFigure( DrawableFigure * fig )
{
  m_pDrawable = fig ;
}
/*------------------------------------------------------------------------------------------*/
