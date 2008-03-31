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

#include "YAxesPositioner.hxx"
#include "DrawableAxes.h"

extern "C"
{
#include "math_graphics.h"
#include "GetProperty.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
YAxesPositioner::YAxesPositioner(DrawableAxes * axes)
  : PlaceTicksStrategy(axes)
{

}
/*---------------------------------------------------------------------------------*/
YAxesPositioner::~YAxesPositioner(void)
{

}
/*---------------------------------------------------------------------------------*/
void YAxesPositioner::computeAxisBounds3D(const double ticksAbscissas[], int nbTicks, double axisStart[3], double axisEnd[3])
{
  sciPointObj * pAxes = m_pDrawer->getDrawedObject();
  sciAxes * ppAxes = pAXES_FEATURE(pAxes);

  // find bounds from ticks positions
  axisStart[0] = ppAxes->vx[0];
  axisStart[1] = ticksAbscissas[0];
  axisStart[2] = 0.0;

  axisEnd[0] = ppAxes->vx[0];
  axisEnd[1] = ticksAbscissas[nbTicks - 1];
  axisEnd[2] = 0.0;
}
/*---------------------------------------------------------------------------------*/
void YAxesPositioner::computeTicksDir3D(double ticksDir[3])
{
  sciAxes * ppAxes = pAXES_FEATURE(m_pDrawer->getDrawedObject());
  // left or right for X axis
  if (ppAxes->dir == 'l')
  {
    ticksDir[0] = -1.0;
  }
  else
  {
    ticksDir[0] = -1.0;
  }

  ticksDir[1] = 0.0;
  ticksDir[2] = 0.0;

}
/*---------------------------------------------------------------------------------*/
void YAxesPositioner::computeTicksLocation3D(const double ticksAbscissas[],
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
    ticksXPos[i] = ppAxes->vx[0];
    ticksYPos[i] = ticksAbscissas[i];
    ticksZPos[i] = 0.0;
  }
  *nbTicksOut = nbTicksIn;
}
/*---------------------------------------------------------------------------------*/

}
