/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines to draw a grayplot object with JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableGrayplotJoGL.hxx"
#include "DrawableGrayplot.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableGrayplotJoGL::DrawableGrayplotJoGL( DrawableGrayplot * drawer )
  : DrawableClippedObjectJoGL(drawer)
{

}
/*---------------------------------------------------------------------------------*/
DrawableGrayplotJoGL::~DrawableGrayplotJoGL( void )
{

}
/*---------------------------------------------------------------------------------*/
void DrawableGrayplotJoGL::drawGrayplot(const double xGrid[], int nbRow,
                                        const double yGrid[], int nbCol,
                                        double zCoord, const int colors[],
                                        int nbColors)
{
  getGrayplotJavaMapper()->drawGrayplot(xGrid, nbRow, yGrid, nbCol,
                                        zCoord, colors, nbColors);
}
/*---------------------------------------------------------------------------------*/
DrawableGrayplot * DrawableGrayplotJoGL::getGrayplotDrawer( void )
{
  return dynamic_cast<DrawableGrayplot *>(getDrawer()) ;
}
/*---------------------------------------------------------------------------------*/
DrawableGrayplotJavaMapper * DrawableGrayplotJoGL::getGrayplotJavaMapper(void)
{
  return dynamic_cast<DrawableGrayplotJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
