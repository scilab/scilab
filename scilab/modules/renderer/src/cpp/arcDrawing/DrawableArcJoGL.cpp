/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependent routines to draw an arc object with JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableArcJoGL.hxx"

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
DrawableArcJoGL::DrawableArcJoGL(DrawableArc * arc)
  : DrawableClippedObjectJoGL(arc) 
{

}
/*---------------------------------------------------------------------------------*/
DrawableArcJoGL::~DrawableArcJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
DrawableArc * DrawableArcJoGL::getArcDrawer( void )
{
  return dynamic_cast<DrawableArc *>(getDrawer());
}
/*---------------------------------------------------------------------------------*/
}
