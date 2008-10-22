/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Strategy for computing best fitting linear bounds from user ones
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "LinearBoundsComputer.hxx"

extern "C"
{
#include "Format.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
void LinearBoundsComputer::applyScaleModification(const double inputBounds[2], double outputBounds[2])
{
  // nothing
  outputBounds[0] = inputBounds[0];
  outputBounds[1] = inputBounds[1];
}
/*------------------------------------------------------------------------------------------*/
void LinearBoundsComputer::applyBestFitting(const double inputBounds[2], double outputBounds[2])
{
  // we use the ticks since it compute correct data bounds
  // TODO rewrite TheTicks to avoid aloso computing ticks

  // variables used by the ticks
  int nbTicks;
  double ticks[20];

  outputBounds[0] = inputBounds[0];
  outputBounds[1] = inputBounds[1];

  TheTicks(&(outputBounds[0]), &(outputBounds[1]), ticks, &nbTicks, FALSE);

  // nothing for now
  outputBounds[0] = ticks[0];
  outputBounds[1] = ticks[nbTicks - 1];
}
/*------------------------------------------------------------------------------------------*/
void LinearBoundsComputer::pointScale(double coord, double * scaledCoord)
{
  // nothing to do, this is linear
  *scaledCoord = coord;
}
/*------------------------------------------------------------------------------------------*/
void LinearBoundsComputer::inversePointScale(double scaledCoord, double * coord)
{
  // nothing to do, this is linear
  *coord = scaledCoord;
}
/*------------------------------------------------------------------------------------------*/
void LinearBoundsComputer::pointScale(double vector[], int vectorLength)
{
  // nothing to do, this is linear
}
/*------------------------------------------------------------------------------------------*/
void LinearBoundsComputer::inversePointScale(double vector[], int vectorLength)
{
  // nothing to do, this is linear
}
/*------------------------------------------------------------------------------------------*/
void LinearBoundsComputer::directionScale(double coord,  double startingPoint, double * scaledCoord)

{
  // nothing to do, this is linear
  *scaledCoord = coord;
}
/*------------------------------------------------------------------------------------------*/
void LinearBoundsComputer::directionScale(double vector[], double startingPoints[], double vectorLength)
{
  // nothing to do, this is linear
}
/*------------------------------------------------------------------------------------------*/
}
