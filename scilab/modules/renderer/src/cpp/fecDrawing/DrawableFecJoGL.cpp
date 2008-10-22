/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependent routines to draw a fec object with JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableFecJoGL.hxx"
#include "DrawableFec.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableFecJoGL::DrawableFecJoGL( DrawableFec * drawer )
  : DrawableClippedObjectJoGL(drawer)
{

}
/*---------------------------------------------------------------------------------*/
DrawableFecJoGL::~DrawableFecJoGL( void )
{

}
/*---------------------------------------------------------------------------------*/
DrawableFec * DrawableFecJoGL::getFecDrawer( void )
{
  return dynamic_cast<DrawableFec *>(getDrawer()) ;
}
/*---------------------------------------------------------------------------------*/
DrawableFecJavaMapper * DrawableFecJoGL::getFecJavaMapper(void)
{
  return dynamic_cast<DrawableFecJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
