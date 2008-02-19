/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for DrawableSurfaceBridge
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableSurfaceBridgeFactory.hxx"
#include "DrawableSurfaceJoGL.hxx"
#include "DrawableSurfaceJavaMapper.hxx"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableSurfaceBridge * DrawableSurfaceBridgeFactory::create( void )
{
  DrawableSurfaceJoGL * imp = new DrawableSurfaceJoGL( m_pDrawable ) ;
  return imp ;
}
/*---------------------------------------------------------------------------------*/
void DrawableSurfaceBridgeFactory::setDrawedSurface( DrawableSurface * poly )
{
  m_pDrawable = poly ;
}
/*---------------------------------------------------------------------------------*/

}
