/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for DrawableFecBridge
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableFecBridgeFactory.hxx"
#include "DrawableFecJoGL.hxx"
#include "DrawableFecJavaMapper.hxx"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableFecBridge * DrawableFecBridgeFactory::create( void )
{
  DrawableFecJoGL * imp = new DrawableFecJoGL( m_pDrawable ) ;
  imp->setJavaMapper(new DrawableFecJavaMapper()) ;
  return imp ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFecBridgeFactory::setDrawedFec( DrawableFec * fec )
{
  m_pDrawable = fec ;
}
/*---------------------------------------------------------------------------------*/

}
