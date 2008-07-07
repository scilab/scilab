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

}
/*------------------------------------------------------------------------------------------*/
YGridDrawerJoGL::~YGridDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
int YGridDrawerJoGL::getGridStyle(void)
{
  sciPointObj * pSubwin = getDrawer()->getDrawedObject();

  int yStyle;
  sciGetGridStyle(pSubwin, NULL, &yStyle, NULL);

  return yStyle;
}
/*------------------------------------------------------------------------------------------*/

}
