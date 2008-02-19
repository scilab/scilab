/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for DrawableFigureImp
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableFigureBridgeFactory.h"
#include "DrawableFigureJoGL.h"


namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableFigureBridge * DrawableFigureBridgeFactory::create( void )
{
  DrawableFigureJoGL * newBridge = new DrawableFigureJoGL( m_pDrawable ) ;
  newBridge->setJavaMapper(new DrawableFigureJavaMapper() ) ;
  return newBridge;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureBridgeFactory::setDrawedFigure( DrawableFigure * fig )
{
  m_pDrawable = fig ;
}
/*---------------------------------------------------------------------------------*/

}
