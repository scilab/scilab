/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for fec objects. Automatically create a fec
 * drawer with the right algorithms from the graphic handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableFecFactory.h"
#include "DrawableFecBridgeFactory.hxx"
#include "FecLineDrawerJoGL.hxx"
#include "FecFacetDrawerJoGL.hxx"
#include "getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject * DrawableFecFactory::create( void )
{
  ConcreteDrawableFec * newFec = new ConcreteDrawableFec( m_pDrawed ) ;
  DrawableFecBridgeFactory fact;
  fact.setDrawedFec(newFec);
  newFec->setDrawableImp(fact.create());
  setStrategies(newFec);

  return newFec;
}
/*---------------------------------------------------------------------------------*/
void DrawableFecFactory::update( void )
{
  setStrategies(dynamic_cast<ConcreteDrawableFec *>(getFecDrawer(m_pDrawed)));
}
/*---------------------------------------------------------------------------------*/
void DrawableFecFactory::setStrategies(ConcreteDrawableFec * fec)
{
  fec->removeDrawingStrategies();
  
  if (sciGetIsDisplayingLines(m_pDrawed))
  {
    fec->addDrawingStrategy(new FecLineDrawerJoGL(fec));
  }

  fec->addDrawingStrategy(new FecFacetDrawerJoGL(fec));

}
/*---------------------------------------------------------------------------------*/

}
