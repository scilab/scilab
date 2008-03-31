/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "XAxesPositioner.hxx"
#include "DrawableAxes.h"

extern "C"
{
#include "math_graphics.h"
#include "GetProperty.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
XAxesPositioner::XAxesPositioner(DrawableAxes * axes)
  : PlaceTicksStrategy(axes)
{

}
/*---------------------------------------------------------------------------------*/
XAxesPositioner::~XAxesPositioner(void)
{

}
/*---------------------------------------------------------------------------------*/
bool XAxesPositioner::isAxisSegmentDrawn(void)
{
  return (pAXES_FEATURE(m_pDrawer->getDrawedObject())->seg == 1);
}
/*---------------------------------------------------------------------------------*/
void XAxesPositioner::computeAxisBounds3D(const double ticksAbscissas[], int nbTicks, double axisStart[3], double axisEnd[3])
{
  sciPointObj * pAxes = m_pDrawer->getDrawedObject();
  sciAxes * ppAxes = pAXES_FEATURE(pAxes);

  // find bounds from ticks positions
  axisStart[0] = ticksAbscissas[0];
  axisStart[1] = ppAxes->vy[0];
  axisStart[2] = 0.0;

  axisEnd[0] = ticksAbscissas[nbTicks - 1];
  axisEnd[1] = ppAxes->vy[0];
  axisEnd[2] = 0.0;
}
/*---------------------------------------------------------------------------------*/
void XAxesPositioner::computeTicksDir3D(double ticksDir[3])
{
  sciAxes * ppAxes = pAXES_FEATURE(m_pDrawer->getDrawedObject());
  // up or down for X axis
  if (ppAxes->dir == 'u')
  {
    ticksDir[1] = 1.0;
  }
  else
  {
    ticksDir[1] = -1.0;
  }

  ticksDir[0] = 0.0;
  ticksDir[2] = 0.0;

}
/*---------------------------------------------------------------------------------*/
void XAxesPositioner::computeTicksLocation3D(const double ticksAbscissas[],
                                             double ticksXPos[],
                                             double ticksYPos[],
                                             double ticksZPos[],
                                             char * labels[],
                                             char * labelsExponents[],
                                             int nbTicksIn,
                                             int * nbTicksOut)
{
  sciAxes * ppAxes = pAXES_FEATURE(m_pDrawer->getDrawedObject());
  for (int i = 0; i < nbTicksIn; i++)
  {
    ticksXPos[i] = ticksAbscissas[i];
    ticksYPos[i] = ppAxes->vy[0];
    ticksZPos[i] = 0.0;
  }
  *nbTicksOut = nbTicksIn;
}
/*---------------------------------------------------------------------------------*/

}
