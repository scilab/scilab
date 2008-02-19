/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for DrawableRectangleBridge
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableSegsBridgeFactory.hxx"
#include "DrawableSegsJoGL.hxx"
#include "DrawableSegsJavaMapper.hxx"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableSegsBridge * DrawableSegsBridgeFactory::create( void )
{
  DrawableSegsJoGL * imp = new DrawableSegsJoGL( m_pDrawable ) ;
  imp->setJavaMapper(new DrawableSegsJavaMapper()) ;
  return imp ;
}
/*---------------------------------------------------------------------------------*/
void DrawableSegsBridgeFactory::setDrawedSegs( DrawableSegs * poly )
{
  m_pDrawable = poly ;
}
/*---------------------------------------------------------------------------------*/

}
