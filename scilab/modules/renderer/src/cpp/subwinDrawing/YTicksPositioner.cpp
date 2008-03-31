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

#include "YTicksPositioner.hxx"

extern "C"
{
#include "math_graphics.h"
#include "GetProperty.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
YTicksPositioner::YTicksPositioner(DrawableSubwin * subwin)
  : PlaceTicksStrategy(subwin)
{

}
/*---------------------------------------------------------------------------------*/
YTicksPositioner::~YTicksPositioner(void)
{

}
/*---------------------------------------------------------------------------------*/
void YTicksPositioner::computeAxisBounds3D(const double ticksAbscissas[], int nbTicks, double axisStart[3], double axisEnd[3])
{
  double zCoordinate = findZCoordinate();
  double xCoordinate = findXCoordinate(zCoordinate);
  computeAxisBounds3D(axisStart, axisEnd, xCoordinate, zCoordinate);
}
/*---------------------------------------------------------------------------------*/
void YTicksPositioner::computeTicksDir3D(double ticksDir[3])
{
  double zCoordinate = findZCoordinate();
  double xCoordinate = findXCoordinate(zCoordinate);
  computeTicksDir3D(ticksDir, xCoordinate, zCoordinate);
}
/*---------------------------------------------------------------------------------*/
void YTicksPositioner::computeTicksLocation3D(const double ticksAbscissas[],
                                              double ticksXPos[],
                                              double ticksYPos[],
                                              double ticksZPos[],
                                              char * labels[],
                                              char * labelsExponents[],
                                              int nbTicksIn,
                                              int * nbTicksOut)
{
  double zCoordinate = findZCoordinate();
  double xCoordinate = findXCoordinate(zCoordinate);
  computeTicksLocation3D(ticksAbscissas, ticksXPos, ticksYPos, ticksZPos, labels, labelsExponents,
                         nbTicksIn, nbTicksOut, xCoordinate, zCoordinate);
}
/*---------------------------------------------------------------------------------*/
void YTicksPositioner::findYTicksDirection(double ticksDirection[3], double xCoordinate, double zCoordinate)
{
  // default ticks direction is along Y axis
  ticksDirection[1] = 0.0;
  ticksDirection[2] = 0.0;
  if (Abs(xCoordinate - getXmin()) <= Abs(xCoordinate - getXmax()))
  {
    // yCoordinate is closer to Ymin
    ticksDirection[0] = getXmin() - getXmax();
  }
  else
  {
    ticksDirection[0] = getXmax() - getXmin();
  }
  normalize3D(ticksDirection);
}
/*---------------------------------------------------------------------------------*/
void YTicksPositioner::computeAxisBounds3D(double axisStart[3], double axisEnd[3], double xCoordinate, double zCoordinate)
{
  axisStart[0] = xCoordinate;
  axisStart[1] = getYmin();
  axisStart[2] = zCoordinate;

  axisEnd[0] = xCoordinate;
  axisEnd[1] = getYmax();
  axisEnd[2] = zCoordinate;
}
/*---------------------------------------------------------------------------------*/
void YTicksPositioner::computeTicksDir3D(double ticksDir[3], double xCoordinate, double zCoordinate)
{
  // find default direction
  findYTicksDirection(ticksDir, xCoordinate, zCoordinate);

  double axisStart[3];
  double axisEnd[3];
  computeAxisBounds3D(axisStart, axisEnd, xCoordinate, zCoordinate);

  // if angle is too small then use Z direction
  if (!checkTicksDirection(ticksDir, axisStart, axisEnd)) {
    getZDirection(ticksDir, zCoordinate);
  }
}
/*---------------------------------------------------------------------------------*/
void YTicksPositioner::computeTicksLocation3D(const double ticksAbscissas[],
                                              double ticksXPos[],
                                              double ticksYPos[],
                                              double ticksZPos[],
                                              char * labels[],
                                              char * labelsExponents[],
                                              int nbTicksIn,
                                              int * nbTicksOut,
                                              double xCoordinate,
                                              double zCoordinate)
{
  *nbTicksOut = 0;
  for (int i = 0; i < nbTicksIn; i++ )
  {
    double yCoordinate = ticksAbscissas[i];
    // check that bounds are within ymin and ymax
    if (yCoordinate >= getYmin() && yCoordinate <= getYmax()) {
      setPosition(ticksXPos, ticksYPos, ticksZPos, xCoordinate, yCoordinate, zCoordinate,
                  labels, labelsExponents, *nbTicksOut, i);
      (*nbTicksOut)++;
    }
  }
}
/*---------------------------------------------------------------------------------*/
void YTicksPositioner::computeGridStartPoints3D(const double ticksAbscissas[],
                                                const double subticksAbscissas[],
                                                double startXPos[],
                                                double startYPos[],
                                                double startZPos[],
                                                int nbTicksIn,
                                                int nbSubicksIn,
                                                int * nbTicksOut)
{
  double zCoordinate = findLowerZCoordinate();
  double xCoordinate = findFrontXCoordinate(zCoordinate);
  computeTicksLocation3D(ticksAbscissas, startXPos, startYPos, startZPos, NULL, NULL,
                         nbTicksIn, nbTicksOut, xCoordinate, zCoordinate);

  char logFlags[3];
  sciGetLogFlags(sciGetParentSubwin(m_pDrawer->getDrawedObject()), logFlags);
  if (logFlags[1] == 'l')
  {
    // add also subticks after ticks
    int nbSubticks = 0;
    computeTicksLocation3D(subticksAbscissas,
                           startXPos + *nbTicksOut,
                           startYPos + *nbTicksOut,
                           startZPos + *nbTicksOut,
                           NULL, NULL,
                           nbSubicksIn, &nbSubticks,
                           xCoordinate, zCoordinate);
    *nbTicksOut += nbSubticks;                     
  }
}
/*---------------------------------------------------------------------------------*/
void YTicksPositioner::computeGridMiddlePoints3D(const double ticksAbscissas[],
                                                 const double subticksAbscissas[],
                                                 double middleXPos[],
                                                 double middleYPos[],
                                                 double middleZPos[],
                                                 int nbTicksIn,
                                                 int nbSubticksIn,
                                                 int * nbTicksOut)
{
  computeGridStartPoints3D(ticksAbscissas, subticksAbscissas, middleXPos, middleYPos, middleZPos,
                           nbTicksIn, nbSubticksIn, nbTicksOut);

  // set new value for X
  double ticksXCoord = middleXPos[0]; // get constant value alonf Y axis
  double xCoordinateMiddle = getOtherXBound(ticksXCoord);

  for (int i = 0; i < *nbTicksOut; i++)
  {
    middleXPos[i] = xCoordinateMiddle;
  }

}
/*---------------------------------------------------------------------------------*/
void YTicksPositioner::computeGridEndPoints3D(const double ticksAbscissas[],
                                              const double subticksAbscissas[],
                                              double endXPos[],
                                              double endYPos[],
                                              double endZPos[],
                                              int nbTicksIn,
                                              int nbSubticksIn,
                                              int * nbTicksOut)
{
  computeGridMiddlePoints3D(ticksAbscissas, subticksAbscissas,
                            endXPos, endYPos, endZPos,
                            nbTicksIn, nbSubticksIn, nbTicksOut);

  // set new value for Z
  double ticksZCoord = endZPos[0]; // get constant value alonf Y axis
  double zCoordinateEnd = getOtherZBound(ticksZCoord);

  for (int i = 0; i < *nbTicksOut; i++)
  {
    endZPos[i] = zCoordinateEnd;
  }

}
/*---------------------------------------------------------------------------------*/
}
