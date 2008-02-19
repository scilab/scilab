/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for DrawableAxesBridge
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableAxesBridgeFactory.hxx"
#include "DrawableAxesJoGL.hxx"
#include "DrawableAxesJavaMapper.hxx"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableAxesBridge * DrawableAxesBridgeFactory::create( void )
{
  DrawableAxesJoGL * newBridge = new DrawableAxesJoGL(m_pDrawable);
  newBridge->setJavaMapper(new DrawableAxesJavaMapper());
  return newBridge;
}
/*---------------------------------------------------------------------------------*/
void DrawableAxesBridgeFactory::setDrawedAxes( DrawableAxes * axes )
{
  m_pDrawable = axes;
}
/*---------------------------------------------------------------------------------*/

}
