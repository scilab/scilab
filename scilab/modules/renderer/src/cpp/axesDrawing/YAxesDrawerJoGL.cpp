/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class drawing Y axis object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "YAxesDrawerJoGL.hxx"
#include "YAxesDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
YAxesDrawerJoGL::YAxesDrawerJoGL(DrawableAxes * axes)
  : TicksDrawerJoGL(axes)
{
  // no choice here, only one position for z ticks
  setJavaMapper(new YAxesDrawerJavaMapper());

}
/*------------------------------------------------------------------------------------------*/
YAxesDrawerJoGL::~YAxesDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
void YAxesDrawerJoGL::initializeDrawing(void)
{
  TicksDrawerJoGL::initializeDrawing();

  sciPointObj * pAxes = getDrawer()->getDrawedObject();
  sciAxes * ppAxes = pAXES_FEATURE(pAxes);

  // specif ty the coordinates of the axis
  getYAxesDrawerJavaMapper()->setConstantCoordinates(ppAxes->vx[0], 0.0);

  getYAxesDrawerJavaMapper()->setTicksDirection(ppAxes->dir);

  getYAxesDrawerJavaMapper()->setAxisLineDrawing(ppAxes->seg == 1);

}
/*------------------------------------------------------------------------------------------*/
YAxesDrawerJavaMapper * YAxesDrawerJoGL::getYAxesDrawerJavaMapper(void)
{
  return dynamic_cast<YAxesDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
}
