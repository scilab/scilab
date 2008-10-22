/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for DrawableTextBridge
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableTextBridgeFactory.hxx"
#include "DrawableTextJoGL.hxx"
#include "DrawableTextJavaMapper.hxx"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableTextBridge * DrawableTextBridgeFactory::create( void )
{
  DrawableTextJoGL * imp = new DrawableTextJoGL( m_pDrawable ) ;
  imp->setJavaMapper(new DrawableTextJavaMapper()) ;
  return imp ;
}
/*---------------------------------------------------------------------------------*/
void DrawableTextBridgeFactory::setDrawedText( DrawableText* poly )
{
  m_pDrawable = poly ;
}
/*---------------------------------------------------------------------------------*/

}
