/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines to draw a
 * polyline object with JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawablePolylineJoGL.h"
#include "DrawablePolyline.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawablePolylineJoGL::DrawablePolylineJoGL( DrawablePolyline * drawer )
  : DrawableClippedObjectJoGL(drawer)
{
  
}
/*---------------------------------------------------------------------------------*/
DrawablePolylineJoGL::~DrawablePolylineJoGL( void )
{
  
}
/*---------------------------------------------------------------------------------*/
DrawablePolyline * DrawablePolylineJoGL::getPolylineDrawer( void )
{
  return dynamic_cast<DrawablePolyline *>(getDrawer()) ;
}
/*---------------------------------------------------------------------------------*/
DrawablePolylineJavaMapper * DrawablePolylineJoGL::getPolylineJavaMapper(void)
{
  return dynamic_cast<DrawablePolylineJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
