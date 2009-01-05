/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Strategy for computing best fitting logarithmic bounds from user ones.
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "LogarithmicBoundsComputer.hxx"

#include <math.h>

extern "C"
{
#include "math_graphics.h"
#include "Format.h"
#include "machine.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
void LogarithmicBoundsComputer::applyScaleModification(const double inputBounds[2], double outputBounds[2])
{
  // get the log of bounds since it will ease computation after
  outputBounds[0] = log10(inputBounds[0]);
  outputBounds[1] = log10(inputBounds[1]);
}
/*------------------------------------------------------------------------------------------*/
void LogarithmicBoundsComputer::applyBestFitting(const double inputBounds[2], double outputBounds[2])
{
  // find bounds such has minBounds+ = 10^x < minBounds < 10^(x+1)
  // and 10^(y-1) < maxBounds < 10^y = maxBounds+
  // at this stage bounds are already logged, so we have
  // x < log10(minBounds) < x + 1
  // y-1 < log10(maxBounds) < y
  //outputBounds[0] = floor(inputBounds[0]);
  //outputBounds[1] = ceil(inputBounds[1]);

  // we use the ticks since it compute correct data bounds
  // TODO rewrite GradLog to avoid also computing ticks

  // variables used by the ticks
  int nbTicks;
  double ticks[20];

  outputBounds[0] = inputBounds[0];
  outputBounds[1] = inputBounds[1];

  GradLog(outputBounds[0], outputBounds[1], ticks, &nbTicks, FALSE);

  // nothing for now
  outputBounds[0] = ticks[0];
  outputBounds[1] = ticks[nbTicks - 1];
}
/*------------------------------------------------------------------------------------------*/
void LogarithmicBoundsComputer::pointScale(double coord, double * scaledCoord)
{
  *scaledCoord = log10(coord);
}
/*------------------------------------------------------------------------------------------*/
void LogarithmicBoundsComputer::inversePointScale(double scaledCoord, double * coord)
{
#ifdef exp10
  *coord = exp10(scaledCoord);
#else
  *coord = pow(10,scaledCoord);
#endif
}
/*------------------------------------------------------------------------------------------*/
void LogarithmicBoundsComputer::pointScale(double vector[], int vectorLength)
{
  for (int i = 0; i < vectorLength; i++)
  {
    vector[i] = log10(vector[i]);
  }
}
/*------------------------------------------------------------------------------------------*/
void LogarithmicBoundsComputer::inversePointScale(double vector[], int vectorLength)
{
  for (int i = 0; i < vectorLength; i++)
  {
#ifdef exp10
    vector[i] = exp10(vector[i]);
#else
	vector[i] = pow(10,vector[i]);
#endif
  }
}
/*------------------------------------------------------------------------------------------*/
void LogarithmicBoundsComputer::directionScale(double coord,  double startingPoint, double * scaledCoord)
{
  // I guess that "/" is faster than "log10"
  *scaledCoord = log10((startingPoint + coord) / startingPoint);
}
/*------------------------------------------------------------------------------------------*/
void LogarithmicBoundsComputer::directionScale(double vector[], double startingPoints[], double vectorLength)
{
  for (int i = 0; i < vectorLength; i++)
  {
    // I guess that "/" is faster than "log10"
    vector[i] = log10((startingPoints[i] + vector[i]) / startingPoints[i]);
  }
}
/*------------------------------------------------------------------------------------------*/
}
