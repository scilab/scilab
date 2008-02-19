/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for DrawableSubwinBridge
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableSubwinBridgeFactory.h"
#include "DrawableSubwinJoGL.h"
#include "DrawableSubwinJavaMapper.hxx"


namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableSubwinBridge * DrawableSubwinBridgeFactory::create( void )
{
  DrawableSubwinJoGL * newBridge =  new DrawableSubwinJoGL(m_pDrawable);
  newBridge->setJavaMapper(new DrawableSubwinJavaMapper());
  return newBridge;
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwinBridgeFactory::setDrawedSubwin( DrawableSubwin * subwin )
{
  m_pDrawable = subwin;
}
/*---------------------------------------------------------------------------------*/

}
