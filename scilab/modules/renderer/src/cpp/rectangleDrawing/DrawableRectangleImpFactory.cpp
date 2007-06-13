/*------------------------------------------------------------------------*/
/* file: DrawableRectangleImpFactory.cpp                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableRectangleImp                                */
/*------------------------------------------------------------------------*/

#include "DrawableRectangleImpFactory.h"
#include "DrawableRectangleJoGL.h"
#include "RectangleFillDrawerJoGL.h"
#include "RectangleLineDrawerJoGL.h"
#include "RectangleMarkDrawerJoGL.h"
extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
DrawableRectangleImp * DrawableRectangleImpFactory::create( void )
{
  DrawableRectangleJoGL * imp = new DrawableRectangleJoGL( m_pDrawable ) ;

  setStrategies( imp ) ;

  return imp ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleImpFactory::update( void )
{
  setStrategies( m_pDrawable->getRectangleImp() ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleImpFactory::setDrawedRectangle( DrawableRectangle * rect )
{
  m_pDrawable = rect ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleImpFactory::setStrategies( DrawableRectangleImp * imp )
{
  imp->removeDrawingStrategies() ;

  // Create rendering algorithms
  sciPointObj * rect = m_pDrawable->getDrawedObject() ;

  if ( sciGetIsFilled( rect ) )
  {
    imp->addDrawingStrategy( new RectangleFillDrawerJoGL( imp ) ) ;
  }

  if ( sciGetIsLine( rect ) )
  {
    imp->addDrawingStrategy( new RectangleLineDrawerJoGL( imp ) ) ;
  }

  if ( sciGetIsMark( rect ) )
  {
    imp->addDrawingStrategy( new RectangleMarkDrawerJoGL( imp ) ) ;
  }
}
/*------------------------------------------------------------------------------------------*/

}
