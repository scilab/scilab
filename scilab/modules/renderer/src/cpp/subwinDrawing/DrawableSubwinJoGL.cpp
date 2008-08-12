/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines to draw a
 * subwin object with JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "DrawableSubwinJoGL.h"
#include "DrawableSubwin.h"
#include "../DrawableObjectJoGL.h"
#include "../DrawableObjectBridge.h"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableSubwinJoGL::DrawableSubwinJoGL( DrawableSubwin * drawer )
  : DrawableObjectJoGL(drawer)
{

}
/*---------------------------------------------------------------------------------*/
DrawableSubwinJoGL::~DrawableSubwinJoGL( void )
{

}
/*---------------------------------------------------------------------------------*/
void DrawableSubwinJoGL::initializeDrawing( void )
{
  sciPointObj * pSubwin = getSubwinDrawer()->getDrawedObject();
  DrawableObjectJoGL::initializeDrawing();
  getSubwinJavaMapper()->setSubwinParameters(sciGetSubwinIndex(pSubwin),
                                             sciGetIs3d(pSubwin) == FALSE);
}
/*---------------------------------------------------------------------------------*/
DrawableSubwin * DrawableSubwinJoGL::getSubwinDrawer( void )
{
  return dynamic_cast<DrawableSubwin *>(getDrawer()) ;
}
/*---------------------------------------------------------------------------------*/
DrawableSubwinJavaMapper * DrawableSubwinJoGL::getSubwinJavaMapper(void)
{
  return dynamic_cast<DrawableSubwinJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
