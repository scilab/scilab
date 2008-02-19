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
#include "../getHandleDrawer.h"
#include "../subwinDrawing/CameraFactory.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject * DrawableRectangleFactory::create( void )
{
  ConcreteDrawableRectangle * newRect = new ConcreteDrawableRectangle( m_pDrawed ) ;
  DrawableRectangleBridgeFactory imp ;
  imp.setDrawedRectangle( newRect ) ;
  newRect->setDrawableImp( imp.create() ) ;

  return newRect ;
}
/*---------------------------------------------------------------------------------*/
void DrawableRectangleFactory::update( void )
{
  DrawableRectangleBridgeFactory imp ;
  imp.setDrawedRectangle( getRectangleDrawer(m_pDrawed) ) ;
  imp.update();
}
/*---------------------------------------------------------------------------------*/

}
