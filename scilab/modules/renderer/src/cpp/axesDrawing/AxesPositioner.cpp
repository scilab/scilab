/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Class specialized in drawing ticks  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "AxesPositioner.hxx"

extern "C"
{
#include "GetProperty.h"
#include "Format.h"
#include "DestroyObjects.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
AxesPositioner::AxesPositioner(DrawableAxes * axes)
{
  m_pAxes = axes;
}
/*------------------------------------------------------------------------------------------*/
AxesPositioner::~AxesPositioner(void)
{
  m_pAxes = NULL;
}
/*------------------------------------------------------------------------------------------*/
void AxesPositioner::getAxisBounds(double startBound[3], double endBound[3])
{
  sciPointObj * pAxes = m_pAxes->getDrawedObject();
  sciAxes * ppAxes = pAXES_FEATURE(pAxes);
  double xStart;
  double xEnd;
  double yStart;
  double yEnd;

  if (isXAxisAligned())
  {
    // axis along X axis
    // Y value is constant
    yStart = ppAxes->vy[0];
    yEnd = ppAxes->vy[0];

    // compute X ticks
    int nbTicks;
    double * tempPos = NULL;
    ComputeXIntervals(pAxes, ppAxes->tics, &tempPos, &nbTicks, 0);

    // get the first and the last one
    xStart = tempPos[0];
    xEnd = tempPos[nbTicks - 1];

    destroyGraphicPointer(tempPos);


  }
  else
  {
    // axis along Y axis
    // X value is constant
    xStart = ppAxes->vx[0];
    xEnd = ppAxes->vx[0];

    // compute Y ticks
    int nbTicks;
    double * tempPos = NULL;
    ComputeXIntervals(pAxes, ppAxes->tics, &tempPos, &nbTicks, 0);

    // get the first and the last one
    yStart = tempPos[0];
    yEnd = tempPos[nbTicks - 1];

    destroyGraphicPointer(tempPos);

  }

  startBound[0] = xStart;
  startBound[1] = yStart;
  startBound[2] = 0.0;

  endBound[0] = xEnd;
  endBound[1] = yEnd;
  endBound[2] = 0.0;

}
/*------------------------------------------------------------------------------------------*/
void AxesPositioner::getTicksDirection(double ticksDir[3])
{
  sciAxes * ppAxes = pAXES_FEATURE(m_pAxes->getDrawedObject());
  switch(ppAxes->dir)
  {
  case 'u':
    // axis along X axis
    // ticks along Y one
    ticksDir[0] = 0.0;
    ticksDir[1] = 1.0;
    break;
  case 'd':
    // axis along X axis
    // ticks along Y one
    ticksDir[0] = 0.0;
    ticksDir[1] = -1.0;
    break;
  case 'l':
    // axis along Y axis
    // ticks along X one
    ticksDir[0] = -1.0;
    ticksDir[1] = 0.0;
    break;
  case 'r':
    // axis along Y axis
    // ticks along X one
    ticksDir[0] = 1.0;
    ticksDir[1] = 0.0;
    break;
  default:
    break;
  }

  ticksDir[2] = 0.0;

  // apply the right number of pixels
  setTicksDirectionLength(ticksDir, sciGetParentSubwin(m_pAxes->getDrawedObject()));

}
/*------------------------------------------------------------------------------------------*/
int AxesPositioner::getRelativeTicksPosition(double ticksPos[], int nbTicks)
{
  double axisStart[3];
  double axisEnd[3];
  getAxisBounds(axisStart, axisEnd);

  double min;
  double max;

  if (isXAxisAligned())
  {
    min = axisStart[0];
    max = axisEnd[0];
  }
  else
  {
    min = axisStart[1];
    max = axisEnd[1];
  }


  for (int i = 0; i < nbTicks; i++)
  {
    ticksPos[i] = (ticksPos[i] - min) / (max - min);
  }
  return nbTicks;
}
/*------------------------------------------------------------------------------------------*/
bool AxesPositioner::isXAxisAligned(void)
{
  sciAxes * ppAxes = pAXES_FEATURE(m_pAxes->getDrawedObject());
  return (ppAxes->nx > ppAxes->ny);
}
/*------------------------------------------------------------------------------------------*/
}
