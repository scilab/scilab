/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for rectangle objects. Automaticaly create a rectangle
 * drawer with the right algorithms from the garphic handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableRectangleFactory.h"
#include "DrawableRectangleBridgeFactory.h"
#include "ConcreteDrawableRectangle.h"
#include "getHandleDrawer.h"
#include "RectangleLineDrawerJoGL.h"
#include "RectangleFillDrawerJoGL.h"
#include "RectangleMarkDrawerJoGL.h"

extern "C"
{
#include "GetProperty.h"
}


namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject * DrawableRectangleFactory::create( void )
{
  ConcreteDrawableRectangle * newRect = new ConcreteDrawableRectangle( m_pDrawed ) ;
  DrawableRectangleBridgeFactory imp ;
  imp.setDrawedRectangle( newRect ) ;
  newRect->setDrawableImp( imp.create() ) ;
  setStrategies(newRect);

  return newRect ;
}
/*---------------------------------------------------------------------------------*/
void DrawableRectangleFactory::update( void )
{
  setStrategies(dynamic_cast<ConcreteDrawableRectangle *>(getRectangleDrawer(m_pDrawed)));
}
/*---------------------------------------------------------------------------------*/
void DrawableRectangleFactory::setStrategies( ConcreteDrawableRectangle * rectangle )
{
  rectangle->removeDrawingStrategies();

  // Create rendering algorithms

  if ( sciGetIsFilled( m_pDrawed ) )
  {
    RectangleFillDrawerJoGL * newFiller = new RectangleFillDrawerJoGL( rectangle ) ;
    rectangle->addDrawingStrategy( newFiller ) ;
  }

  if ( sciGetIsLine( m_pDrawed ) )
  {
    RectangleLineDrawerJoGL * newLiner = new RectangleLineDrawerJoGL(rectangle);
    rectangle->addDrawingStrategy( newLiner ) ;
  }

  if ( sciGetIsMark( m_pDrawed ) )
  {
    RectangleMarkDrawerJoGL * newMarker = new RectangleMarkDrawerJoGL(rectangle);
    rectangle->addDrawingStrategy( newMarker ) ;
  }
}
/*---------------------------------------------------------------------------------*/
}
