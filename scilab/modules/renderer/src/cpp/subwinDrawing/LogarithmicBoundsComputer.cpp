/*------------------------------------------------------------------------*/
/* file: LogarithmicBoundsComputer.cpp                                    */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy for computing best fitting logarithmic bounds from     */
/*        user ones.                                                      */
/*------------------------------------------------------------------------*/

#include "LogarithmicBoundsComputer.hxx"

#include <math.h>

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
  // nothing for now
  outputBounds[0] = inputBounds[0];
  outputBounds[1] = inputBounds[1];
}
/*------------------------------------------------------------------------------------------*/
void LogarithmicBoundsComputer::pointScale(double coord, double * scaledCoord)
{
  *scaledCoord = log10(coord);
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
void LogarithmicBoundsComputer::directionScale(double coord,  double startingPoint, double * scaledCoord)
{
  *scaledCoord = log10(startingPoint + coord) - log10(startingPoint);
}
/*------------------------------------------------------------------------------------------*/
void LogarithmicBoundsComputer::directionScale(double vector[], double startingPoints[], double vectorLength)
{
  for (int i = 0; i < vectorLength; i++)
  {
    vector[i] = log10(startingPoints[i] + vector[i]) - log10(startingPoints[i]);
  }
}
/*------------------------------------------------------------------------------------------*/
}
