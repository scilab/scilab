/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class specialized in drawing grid using JoGL for Y axis
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "YGridDrawerJoGL.hxx"
#include "YGridDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
YGridDrawerJoGL::YGridDrawerJoGL(DrawableSubwin * subwin)
  : GridDrawerJoGL(subwin)
{
  setJavaMapper(new YGridDrawerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
YGridDrawerJoGL::~YGridDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
void YGridDrawerJoGL::initializeDrawing(void)
{
  GridDrawerJoGL::initializeDrawing();

  sciPointObj * pSubwin = getDrawer()->getDrawedObject();

  int yStyle;
  sciGetGridStyle(pSubwin, NULL, &yStyle, NULL);

  getGridDrawerJavaMapper()->setGridParameters(yStyle, (float)sciGetLineWidth(pSubwin));

}
/*------------------------------------------------------------------------------------------*/

}
