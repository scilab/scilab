/*------------------------------------------------------------------------*/
/* file: DrawableRectangleBridgeFactory.cpp                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableRectangleImp                                */
/*------------------------------------------------------------------------*/

#include "DrawableRectangleBridgeFactory.h"
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
DrawableRectangleBridge * DrawableRectangleBridgeFactory::create( void )
{
  DrawableRectangleJoGL * imp = new DrawableRectangleJoGL( m_pDrawable ) ;

  setStrategies( imp ) ;

  return imp ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleBridgeFactory::update( void )
{
  setStrategies( m_pDrawable->getRectangleImp() ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleBridgeFactory::setDrawedRectangle( DrawableRectangle * rect )
{
  m_pDrawable = rect ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleBridgeFactory::setStrategies( DrawableRectangleBridge * imp )
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
