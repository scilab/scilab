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

#include "ZTicksPositioner.hxx"

extern "C"
{
#include "math_graphics.h"
#include "GetProperty.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
ZTicksPositioner::ZTicksPositioner(DrawableSubwin * subwin)
  : PlaceTicksStrategy(subwin)
{

}
/*---------------------------------------------------------------------------------*/
ZTicksPositioner::~ZTicksPositioner(void)
{

}
/*---------------------------------------------------------------------------------*/
void ZTicksPositioner::computeAxisBounds3D(const double ticksAbscissas[], int nbTicks, double axisStart[3], double axisEnd[3])
{
  double xCoordinate = findXCoordinate();
  double yCoordinate = findYCoordinate();
  computeAxisBounds3D(axisStart, axisEnd, xCoordinate, yCoordinate);
}
/*---------------------------------------------------------------------------------*/
void ZTicksPositioner::computeTicksDir3D(double ticksDir[3])
{
  double xCoordinate = findXCoordinate();
  double yCoordinate = findYCoordinate();
  computeTicksDir3D(ticksDir, xCoordinate, yCoordinate);
}
/*---------------------------------------------------------------------------------*/
void ZTicksPositioner::computeTicksLocation3D(const double ticksAbscissas[],
                                              double ticksXPos[],
                                              double ticksYPos[],
                                              double ticksZPos[],
                                              char * labels[],
                                              char * labelsExponents[],
                                              int nbTicksIn,
                                              int * nbTicksOut)
{
  double xCoordinate = findXCoordinate();
  double yCoordinate = findYCoordinate();
  computeTicksLocation3D(ticksAbscissas, ticksXPos, ticksYPos, ticksZPos, labels, labelsExponents,
                         nbTicksIn, nbTicksOut, xCoordinate, yCoordinate);
}
/*---------------------------------------------------------------------------------*/
void ZTicksPositioner::findZTicksDirection(double ticksDirection[3], double xCoordinate, double yCoordinate)
{
  // we must find wether Z axis ticks are directed by X axis or Y axis
  // Actually Z axis is directed by the axis with which it shares on edge
  // when X and Y axis are in defautl mode, ie bottom and left


  // get the three coordinates of the edge which is common for X and Y axis in default mode
  double zCoordFront = findLowerZCoordinate();

  // check wether the ticks are OK
  double axisStart[3];
  double axisEnd[3];
  computeAxisBounds3D(axisStart, axisEnd, xCoordinate, yCoordinate);

  ticksDirection[2] = 0.0;
  if (isSharingEndWithXaxis(zCoordFront, xCoordinate))
  {
    ticksDirection[0] = xCoordinate - findFrontXCoordinate(zCoordFront);
    ticksDirection[1] = 0.0;

    // if not ok, then inverse
    if (!checkTicksDirection(ticksDirection, axisStart, axisEnd))
    {
      ticksDirection[0] = 0.0;
      ticksDirection[1] = yCoordinate - findFrontYCoordinate(zCoordFront);
    }

  }
  else
  {
    ticksDirection[0] = 0.0;
    ticksDirection[1] = yCoordinate - findFrontYCoordinate(zCoordFront);

    // if not ok, then inverse
    if (!checkTicksDirection(ticksDirection, axisStart, axisEnd))
    {
      ticksDirection[0] = xCoordinate - findFrontXCoordinate(zCoordFront);
      ticksDirection[1] = 0.0;
    }
  }

  normalize3D(ticksDirection);
}
/*---------------------------------------------------------------------------------*/
bool ZTicksPositioner::isSharingEndWithXaxis(double zCoordFront, double xCoordinate)
{
  // get the three coordinates of the edge whuch is common for X and Y axis in default mode
  double xCoordFront = findFrontXCoordinate(zCoordFront);
  double xCoordBack  = getOtherXBound(xCoordFront);

  // equivalent to return xCoordinate == xCoordBack
  return (Abs(xCoordinate - xCoordBack) < Abs(xCoordinate - xCoordFront));
}
/*---------------------------------------------------------------------------------*/
void ZTicksPositioner::computeAxisBounds3D(double axisStart[3], double axisEnd[3], double xCoordinate, double yCoordinate)
{
  axisStart[0] = xCoordinate;
  axisStart[1] = yCoordinate;
  axisStart[2] = getZmin();

  axisEnd[0] = xCoordinate;
  axisEnd[1] = yCoordinate;
  axisEnd[2] = getZmax();
}
/*---------------------------------------------------------------------------------*/
void ZTicksPositioner::computeTicksDir3D(double ticksDir[3], double xCoordinate, double yCoordinate)
{
  // find default direction
  findZTicksDirection(ticksDir, xCoordinate, yCoordinate);
}
/*---------------------------------------------------------------------------------*/
void ZTicksPositioner::computeTicksLocation3D(const double ticksAbscissas[],
                                              double ticksXPos[],
                                              double ticksYPos[],
                                              double ticksZPos[],
                                              char * labels[],
                                              char * labelsExponents[],
                                              int nbTicksIn,
                                              int * nbTicksOut,
                                              double xCoordinate,
                                              double yCoordinate)
{
  *nbTicksOut = 0;
  for (int i = 0; i < nbTicksIn; i++ )
  {
    double zCoordinate = ticksAbscissas[i];
    // check that bounds are within ymin and ymax
    if (zCoordinate >= getZmin() && zCoordinate <= getZmax()) {
      setPosition(ticksXPos, ticksYPos, ticksZPos, xCoordinate, yCoordinate, zCoordinate,
                  labels, labelsExponents, *nbTicksOut, i);
      (*nbTicksOut)++;
    }
  }
}
/*---------------------------------------------------------------------------------*/
void ZTicksPositioner::computeGridStartPoints3D(const double ticksAbscissas[],
                                                const double subticksAbscissas[],
                                                double startXPos[],
                                                double startYPos[],
                                                double startZPos[],
                                                int nbTicksIn,
                                                int nbSubicksIn,
                                                int * nbTicksOut)
{
  double xCoordinate = findLeftMostXCoordinate();
  double yCoordinate = findLeftMostYCoordinate();
  computeTicksLocation3D(ticksAbscissas, startXPos, startYPos, startZPos, NULL, NULL,
                         nbTicksIn, nbTicksOut, xCoordinate, yCoordinate);

  char logFlags[3];
  sciGetLogFlags(sciGetParentSubwin(m_pDrawer->getDrawedObject()), logFlags);
  if (logFlags[2] == 'l')
  {
    // add also subticks after ticks
    int nbSubticks = 0;
    computeTicksLocation3D(subticksAbscissas,
                           startXPos + *nbTicksOut,
                           startYPos + *nbTicksOut,
                           startZPos + *nbTicksOut,
                           NULL, NULL,
                           nbSubicksIn, &nbSubticks,
                           xCoordinate, yCoordinate);
    *nbTicksOut += nbSubticks;                     
  }
}
/*---------------------------------------------------------------------------------*/
void ZTicksPositioner::computeGridMiddlePoints3D(const double ticksAbscissas[],
                                                 const double subticksAbscissas[],
                                                 double middleXPos[],
                                                 double middleYPos[],
                                                 double middleZPos[],
                                                 int nbTicksIn,
                                                 int nbSubicksIn,
                                                 int * nbTicksOut)
{
  computeGridStartPoints3D(ticksAbscissas, subticksAbscissas, middleXPos, middleYPos, middleZPos,
                         nbTicksIn, nbSubicksIn, nbTicksOut);


  // middle points are on the opposite of  X or Y axis
  double zCoordFront = findLowerZCoordinate();
  double ticksXCoord = middleXPos[0];
  double ticksYCoord = middleYPos[0];
  if (isSharingEndWithXaxis(zCoordFront, ticksXCoord))
  {
    // inverse y
    double yCoordinateMiddle = getOtherYBound(ticksYCoord);

    for (int i = 0; i < *nbTicksOut; i++)
    {
      middleYPos[i] = yCoordinateMiddle;
    }
  }
  else
  {
    // inverse x
    double xCoordinateMiddle = getOtherXBound(ticksXCoord);

    for (int i = 0; i < *nbTicksOut; i++)
    {
      middleXPos[i] = xCoordinateMiddle;
    }
  }
  

}
/*---------------------------------------------------------------------------------*/
void ZTicksPositioner::computeGridEndPoints3D(const double ticksAbscissas[],
                                              const double subticksAbscissas[],
                                              double endXPos[],
                                              double endYPos[],
                                              double endZPos[],
                                              int nbTicksIn,
                                              int nbSubicksIn,
                                              int * nbTicksOut)
{

  computeGridStartPoints3D(ticksAbscissas, subticksAbscissas, endXPos, endYPos, endZPos,
                           nbTicksIn, nbSubicksIn, nbTicksOut);

  // inverse X and Y
  double ticksXCoord = endXPos[0];
  double ticksYCoord = endYPos[0];
  double xCoordinateEnd = getOtherXBound(ticksXCoord);
  double yCoordinateEnd = getOtherYBound(ticksYCoord);

  for (int i = 0; i < *nbTicksOut; i++)
  {
    endXPos[i] = xCoordinateEnd;
    endYPos[i] = yCoordinateEnd;
  }



  // set new value for Z
  double ticksZCoord = endZPos[0]; // get constant value along Y axis
  double zCoordinateEnd = getOtherZBound(ticksZCoord);

  

}
/*---------------------------------------------------------------------------------*/
double ZTicksPositioner::findXCoordinate(void)
{
  return findLeftMostXCoordinate();
}
/*---------------------------------------------------------------------------------*/
double ZTicksPositioner::findYCoordinate(void)
{
  return findLeftMostYCoordinate();
}
/*---------------------------------------------------------------------------------*/

}
