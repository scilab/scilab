/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependent routines to draw a
 * text object with JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "DrawableTextJoGL.hxx"
#include "DrawableText.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableTextJoGL::DrawableTextJoGL( DrawableText * drawer )
  : DrawableClippedObjectJoGL(drawer)
{

}
/*---------------------------------------------------------------------------------*/
DrawableTextJoGL::~DrawableTextJoGL( void )
{

}
/*---------------------------------------------------------------------------------*/
DrawableText * DrawableTextJoGL::getTextDrawer( void )
{
  return dynamic_cast<DrawableText *>(getDrawer()) ;
}
/*---------------------------------------------------------------------------------*/
DrawableTextJavaMapper * DrawableTextJoGL::getTextJavaMapper(void)
{
  return dynamic_cast<DrawableTextJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
