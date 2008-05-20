/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines to draw a        
 * rectangle object with JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <list>
#include "../DrawableObjectJoGL.h"
#include "DrawableRectangle.h"
#include "DrawableRectangleJoGL.h"


namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
DrawableRectangleJoGL::DrawableRectangleJoGL( DrawableRectangle * drawer )
  : DrawableClippedObjectJoGL(drawer)
{

}
/*---------------------------------------------------------------------------------*/
DrawableRectangleJoGL::~DrawableRectangleJoGL( void )
{

}
/*---------------------------------------------------------------------------------*/
DrawableRectangle * DrawableRectangleJoGL::getRectangleDrawer( void )
{
  return dynamic_cast<DrawableRectangle *>(getDrawer()) ;
}
/*---------------------------------------------------------------------------------*/
DrawableRectangleJavaMapper * DrawableRectangleJoGL::getRectangleJavaMapper(void)
{
  return dynamic_cast<DrawableRectangleJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
