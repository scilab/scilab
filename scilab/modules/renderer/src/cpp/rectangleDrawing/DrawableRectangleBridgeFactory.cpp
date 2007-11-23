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
#include "DrawableRectangleJavaMapper.hxx"
#include "RectangleFillDrawerJavaMapper.hxx"
#include "RectangleLineDrawerJavaMapper.hxx"
#include "RectangleMarkDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
DrawableRectangleBridge * DrawableRectangleBridgeFactory::create( void )
{
  DrawableRectangleJoGL * imp = new DrawableRectangleJoGL( m_pDrawable ) ;
  
  imp->setJavaMapper(new DrawableRectangleJavaMapper());

  setStrategies(imp) ;

  return imp ;
}
/*---------------------------------------------------------------------------------*/
void DrawableRectangleBridgeFactory::update( void )
{
  setStrategies( m_pDrawable->getRectangleImp() ) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableRectangleBridgeFactory::setDrawedRectangle( DrawableRectangle * rect )
{
  m_pDrawable = rect ;
}
/*---------------------------------------------------------------------------------*/
void DrawableRectangleBridgeFactory::setStrategies( DrawableRectangleBridge * imp )
{
  imp->removeDrawingStrategies() ;

  // Create rendering algorithms
  sciPointObj * rect = m_pDrawable->getDrawedObject() ;

  if ( sciGetIsFilled( rect ) )
  {
    RectangleFillDrawerJoGL * newFiller = new RectangleFillDrawerJoGL( imp ) ;
    newFiller->setJavaMapper(new RectangleFillDrawerJavaMapper());
    imp->addDrawingStrategy( newFiller ) ;
  }

  if ( sciGetIsLine( rect ) )
  {
    RectangleLineDrawerJoGL * newLiner = new RectangleLineDrawerJoGL(imp);
    newLiner->setJavaMapper(new RectangleLineDrawerJavaMapper());
    imp->addDrawingStrategy( newLiner ) ;
  }

  if ( sciGetIsMark( rect ) )
  {
    RectangleMarkDrawerJoGL * newMarker = new RectangleMarkDrawerJoGL(imp);
    newMarker->setJavaMapper(new RectangleMarkDrawerJavaMapper());
    imp->addDrawingStrategy( newMarker ) ;
  }
}
/*---------------------------------------------------------------------------------*/

}
