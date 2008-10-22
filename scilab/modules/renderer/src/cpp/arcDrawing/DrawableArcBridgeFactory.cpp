/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for DrawableArcBridge
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableArcBridgeFactory.hxx"
#include "DrawableArcJoGL.hxx"
#include "DrawableArcJavaMapper.hxx"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableArcBridge * DrawableArcBridgeFactory::create( void )
{
  DrawableArcJoGL * newBridge = new DrawableArcJoGL(m_pDrawable);
  newBridge->setJavaMapper(new DrawableArcJavaMapper());
  return newBridge;
}
/*---------------------------------------------------------------------------------*/
void DrawableArcBridgeFactory::setDrawedArc( DrawableArc * arc )
{
  m_pDrawable = arc;
}
/*---------------------------------------------------------------------------------*/

}
