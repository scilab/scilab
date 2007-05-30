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
  
  // Create rendering algorithms
  sciPointObj * rect = m_pDrawable->getDrawedObject() ;

  if ( sciGetIsLine( rect ) )
  {
    imp->addDrawingStrategy( new RectangleLineDrawerJoGL( imp ) ) ;
  }

  if ( sciGetIsFilled( rect ) )
  {
    imp->addDrawingStrategy( new RectangleFillDrawerJoGL( imp ) ) ;
  }

  if ( sciGetIsMark( rect ) )
  {
    imp->addDrawingStrategy( new RectangleMarkDrawerJoGL( imp ) ) ;
  }

  return imp ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleImpFactory::setDrawedRectangle( DrawableRectangle * rect )
{
  m_pDrawable = rect ;
}
/*------------------------------------------------------------------------------------------*/

}
