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

#include "XAxesDrawerJoGL.hxx"
#include "XAxesDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
XAxesDrawerJoGL::XAxesDrawerJoGL(DrawableAxes * axes)
  : TicksDrawerJoGL(axes)
{
  // no choice here, only one position for z ticks
  setJavaMapper(new XAxesDrawerJavaMapper());

}
/*------------------------------------------------------------------------------------------*/
XAxesDrawerJoGL::~XAxesDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
void XAxesDrawerJoGL::initializeDrawing(void)
{
  TicksDrawerJoGL::initializeDrawing();

  sciPointObj * pAxes = getDrawer()->getDrawedObject();
  sciAxes * ppAxes = pAXES_FEATURE(pAxes);

  // specif ty the coordinates of the axis
  getXAxesDrawerJavaMapper()->setConstantCoordinates(ppAxes->vy[0], 0.0);

  getXAxesDrawerJavaMapper()->setTicksDirection(ppAxes->dir);

  getXAxesDrawerJavaMapper()->setAxisLineDrawing(ppAxes->seg == 1);

}
/*------------------------------------------------------------------------------------------*/
XAxesDrawerJavaMapper * XAxesDrawerJoGL::getXAxesDrawerJavaMapper(void)
{
  return dynamic_cast<XAxesDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
}
