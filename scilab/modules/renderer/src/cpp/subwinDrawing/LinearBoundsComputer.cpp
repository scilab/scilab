/*------------------------------------------------------------------------*/
/* file: LinearBoundsComputer.cpp                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy for computing best fitting linear bounds from user     */
/*        ones                                                            */
/*------------------------------------------------------------------------*/

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
