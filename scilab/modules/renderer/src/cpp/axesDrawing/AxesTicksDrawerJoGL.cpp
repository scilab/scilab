/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class drawing X axis object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "AxesTicksDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
AxesTicksDrawerJoGL::AxesTicksDrawerJoGL(DrawableAxes * axes)
  : TicksDrawerJoGL(axes)
{

}
/*------------------------------------------------------------------------------------------*/
AxesTicksDrawerJoGL::~AxesTicksDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
void AxesTicksDrawerJoGL::initializeDrawing(void)
{
  TicksDrawerJoGL::initializeDrawing();

  sciPointObj * pAxes = getDrawer()->getDrawedObject();
  sciAxes * ppAxes = pAXES_FEATURE(pAxes);

  // specify if the axes line should be drawn or not
  getTicksDrawerJavaMapper()->setAxisLineDrawing(ppAxes->seg == 1);

}
/*------------------------------------------------------------------------------------------*/
}
