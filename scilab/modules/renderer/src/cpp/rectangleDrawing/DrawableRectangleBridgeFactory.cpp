/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for DrawableRectangleImp
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
