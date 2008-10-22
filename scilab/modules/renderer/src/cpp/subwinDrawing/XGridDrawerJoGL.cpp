/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class specialized in drawing grid using JoGL for X axis
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "XGridDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
XGridDrawerJoGL::XGridDrawerJoGL(DrawableSubwin * subwin)
  : GridDrawerJoGL(subwin)
{

}
/*------------------------------------------------------------------------------------------*/
XGridDrawerJoGL::~XGridDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
int XGridDrawerJoGL::getGridStyle(void)
{
  sciPointObj * pSubwin = getDrawer()->getDrawedObject();

  int xStyle;
  sciGetGridStyle(pSubwin, &xStyle, NULL, NULL);

  return xStyle;
}
/*------------------------------------------------------------------------------------------*/


}
