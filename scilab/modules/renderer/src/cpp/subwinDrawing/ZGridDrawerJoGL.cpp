/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class specialized in drawing grid using JoGL for Z axis 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ZGridDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
ZGridDrawerJoGL::ZGridDrawerJoGL(DrawableSubwin * subwin)
  : GridDrawerJoGL(subwin)
{

}
/*------------------------------------------------------------------------------------------*/
ZGridDrawerJoGL::~ZGridDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
int ZGridDrawerJoGL::getGridStyle(void)
{
  sciPointObj * pSubwin = getDrawer()->getDrawedObject();

  int zStyle;
  sciGetGridStyle(pSubwin, NULL, NULL, &zStyle);

  return zStyle;
}
/*------------------------------------------------------------------------------------------*/

}
