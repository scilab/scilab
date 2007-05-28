/*------------------------------------------------------------------------*/
/* file: DrawableFigureImpFactory.cpp                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableFigureImp                                   */
/*------------------------------------------------------------------------*/

#include "DrawableFigureImpFactory.h"
#ifdef WITH_JAVA
#include "DrawableFigureJoGL.h"
#endif

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableFigureImp * DrawableFigureImpFactory::create( void )
{
#ifdef WITH_JAVA
  return new DrawableFigureJoGL( m_pDrawable ) ;
#else
  return NULL;
#endif
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureImpFactory::setDrawedFigure( DrawableFigure * fig )
{
  m_pDrawable = fig ;
}
/*------------------------------------------------------------------------------------------*/

}
