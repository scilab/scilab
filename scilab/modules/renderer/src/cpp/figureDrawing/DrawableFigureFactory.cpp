/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for figure objects. Automaticaly create a figure
 * drawer with the right algorithms from the garphic handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableFigureFactory.h"
#include "ConcreteDrawableFigure.h"
#include "DrawableFigureBridge.h"
#include "DrawableFigureBridgeFactory.h"
#include "GraphicSynchronization/GraphicSynchronizerFactory.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject * DrawableFigureFactory::create( void )
{
  DrawableFigure * newFig = new ConcreteDrawableFigure( m_pDrawed ) ;

  // create bridge
  DrawableFigureBridgeFactory imp ;
  imp.setDrawedFigure( newFig ) ;

  newFig->setDrawableImp( imp.create() ) ;

  // create synchronizer
  GraphicSynchronizerFactory fact;
  newFig->setSynchronizer(fact.createLocalSynchronizer());

  newFig->createVisualFigure();

  return newFig ;

}
/*---------------------------------------------------------------------------------*/
void DrawableFigureFactory::update( void )
{
  // nothing for now
}
/*---------------------------------------------------------------------------------*/

}
