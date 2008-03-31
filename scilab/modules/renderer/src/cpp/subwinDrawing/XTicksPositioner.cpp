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

#include "XTicksPositioner.hxx"

extern "C"
{
#include "math_graphics.h"
#include "GetProperty.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
XTicksPositioner::XTicksPositioner(DrawableSubwin * subwin)
  : PlaceTicksStrategy(subwin)
{

}
/*---------------------------------------------------------------------------------*/
XTicksPositioner::~XTicksPositioner(void)
{

}
/*---------------------------------------------------------------------------------*/
void XTicksPositioner::computeAxisBounds3D(const double ticksAbscissas[], int nbTicks, double axisStart[3], double axisEnd[3])
{
  double zCoordinate = findZCoordinate();
  double yCoordinate = findYCoordinate(zCoordinate);
  computeAxisBounds3D(axisStart, axisEnd, yCoordinate, zCoordinate);
}
/*---------------------------------------------------------------------------------*/
void XTicksPositioner::computeTicksDir3D(double ticksDir[3])
{
  double zCoordinate = findZCoordinate();
  double yCoordinate = findYCoordinate(zCoordinate);
  computeTicksDir3D(ticksDir, yCoordinate, zCoordinate);
}
/*---------------------------------------------------------------------------------*/
void XTicksPositioner::computeTicksLocation3D(const double ticksAbscissas[],
                                              double ticksXPos[],
                                              double ticksYPos[],
                                              double ticksZPos[],
                                              char * labels[],
                                              char * labelsExponents[],
                                              int nbTicksIn,
                                              int * nbTicksOut)
{
  double zCoordinate = findZCoordinate();
  double yCoordinate = findYCoordinate(zCoordinate);
  computeTicksLocation3D(ticksAbscissas, ticksXPos, ticksYPos, ticksZPos, labels, labelsExponents,
                         nbTicksIn, nbTicksOut, yCoordinate, zCoordinate);
}
/*---------------------------------------------------------------------------------*/
void XTicksPositioner::findXTicksDirection(double ticksDirection[3], double yCoordinate, double zCoordinate)
{
  // default ticks direction is along Y axis
  ticksDirection[0] = 0.0;
  ticksDirection[2] = 0.0;
  if (Abs(yCoordinate - getYmin()) <= Abs(yCoordinate - getYmax()))
  {
    // yCoordinate is closer to Ymin
    ticksDirection[1] = getYmin() - getYmax();
  }
  else
  {
    ticksDirection[1] = getYmax() - getYmin();
  }
  normalize3D(ticksDirection);
}
/*---------------------------------------------------------------------------------*/
void XTicksPositioner::computeAxisBounds3D(double axisStart[3], double axisEnd[3], double yCoordinate, double zCoordinate)
{
  axisStart[0] = getXmin();
  axisStart[1] = yCoordinate;
  axisStart[2] = zCoordinate;

  axisEnd[0] = getXmax();
  axisEnd[1] = yCoordinate;
  axisEnd[2] = zCoordinate;
}
/*---------------------------------------------------------------------------------*/
void XTicksPositioner::computeTicksDir3D(double ticksDir[3], double yCoordinate, double zCoordinate)
{
  // find default direction
  findXTicksDirection(ticksDir, yCoordinate, zCoordinate);

  double axisStart[3];
  double axisEnd[3];
  computeAxisBounds3D(axisStart, axisEnd, yCoordinate, zCoordinate);

  // if angle is too small then use Z direction
  if (!checkTicksDirection(ticksDir, axisStart, axisEnd)) {
    getZDirection(ticksDir, zCoordinate);
  }
}
/*---------------------------------------------------------------------------------*/
void XTicksPositioner::computeTicksLocation3D(const double ticksAbscissas[],
                                              double ticksXPos[],
                                              double ticksYPos[],
                                              double ticksZPos[],
                                              char * labels[],
                                              char * labelsExponents[],
                                              int nbTicksIn,
                                              int * nbTicksOut,
                                              double yCoordinate,
                                              double zCoordinate)
{
  *nbTicksOut = 0;
  for (int i = 0; i < nbTicksIn; i++ )
  {
    double xCoordinate = ticksAbscissas[i];
    // check that bounds are within xmin and xmax
    if (xCoordinate >= getXmin() && xCoordinate <= getXmax()) {
      setPosition(ticksXPos, ticksYPos, ticksZPos, xCoordinate, yCoordinate, zCoordinate,
                  labels, labelsExponents, *nbTicksOut, i);
      (*nbTicksOut)++;
    }
  }
}
/*---------------------------------------------------------------------------------*/
void XTicksPositioner::computeGridStartPoints3D(const double ticksAbscissas[],
                                                const double subticksAbscissas[],
                                                double startXPos[],
                                                double startYPos[],
                                                double startZPos[],
                                                int nbTicksIn,
                                                int nbSubticksIn,
                                                int * nbTicksOut)
{
  double zCoordinate = findLowerZCoordinate();
  double yCoordinate = findFrontYCoordinate(zCoordinate);
  computeTicksLocation3D(ticksAbscissas, startXPos, startYPos, startZPos, NULL, NULL,
                         nbTicksIn, nbTicksOut, yCoordinate, zCoordinate);

  char logFlags[3];
  sciGetLogFlags(sciGetParentSubwin(m_pDrawer->getDrawedObject()), logFlags);
  if (logFlags[0] == 'l')
  {
    // add also subticks after ticks
    int nbSubticks = 0;
    computeTicksLocation3D(subticksAbscissas,
                           startXPos + *nbTicksOut,
                           startYPos + *nbTicksOut,
                           startZPos + *nbTicksOut,
                           NULL, NULL,
                           nbSubticksIn, &nbSubticks,
                           yCoordinate, zCoordinate);
    *nbTicksOut += nbSubticks;                     
  }



}
/*---------------------------------------------------------------------------------*/
void XTicksPositioner::computeGridMiddlePoints3D(const double ticksAbscissas[],
                                                 const double subticksAbscissas[],
                                                 double middleXPos[],
                                                 double middleYPos[],
                                                 double middleZPos[],
                                                 int nbTicksIn,
                                                 int nbSubticksIn,
                                                 int * nbTicksOut)
{
  

  computeGridStartPoints3D(ticksAbscissas, subticksAbscissas,
                           middleXPos, middleYPos, middleZPos, nbTicksIn, nbSubticksIn, nbTicksOut);


  // set new value for Y
  double ticksYCoord = middleYPos[0]; // get constant value alonf Y axis
  double yCoordinateMiddle = getOtherYBound(ticksYCoord);

  for (int i = 0; i < *nbTicksOut; i++)
  {
    middleYPos[i] = yCoordinateMiddle;
  }

}
/*---------------------------------------------------------------------------------*/
void XTicksPositioner::computeGridEndPoints3D(const double ticksAbscissas[],
                                              const double subticksAbscissas[],
                                              double endXPos[],
                                              double endYPos[],
                                              double endZPos[],
                                              int nbTicksIn,
                                              int nbSubticksIn,
                                              int * nbTicksOut)
{
  computeGridMiddlePoints3D(ticksAbscissas, subticksAbscissas,
                            endXPos, endYPos, endZPos, nbTicksIn, nbSubticksIn, nbTicksOut);

  // set new value for Z
  double ticksZCoord = endZPos[0]; // get constant value alonf Z axis
  double zCoordinateEnd = getOtherZBound(ticksZCoord);

  for (int i = 0; i < *nbTicksOut; i++)
  {
    endZPos[i] = zCoordinateEnd;
  }

}
/*---------------------------------------------------------------------------------*/
}
