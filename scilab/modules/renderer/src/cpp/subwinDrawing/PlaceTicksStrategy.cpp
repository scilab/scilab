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

#include "PlaceTicksStrategy.hxx"
#include "getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
#include "math_graphics.h"
}

/**
 * Length in pixel of ticks
 */
#define TICKS_PIXEL_LENGTH 0.02
#define MAX_COS 0.99

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
PlaceTicksStrategy::PlaceTicksStrategy(DrawableObject * subwin)
{
  m_pDrawer = subwin;
}
/*---------------------------------------------------------------------------------*/
PlaceTicksStrategy::~PlaceTicksStrategy(void)
{
  m_pDrawer = NULL;
}
/*---------------------------------------------------------------------------------*/
void PlaceTicksStrategy::computeAxisBounds(const double ticksAbscissas[], int nbTicks, double axisStart[3], double axisEnd[3])
{
  updateDataBounds();

  // get bounds in 3D
  computeAxisBounds3D(ticksAbscissas, nbTicks, axisStart, axisEnd);

  // convert them in pixel coordinates
  getPixelCoordinates(axisStart, axisStart);
  getPixelCoordinates(axisEnd, axisEnd);
}
/*---------------------------------------------------------------------------------*/
void PlaceTicksStrategy::computeTicksDir(double ticksDir[3])
{

  updateDataBounds();

  // get normalized direction in 3D
  computeTicksDir3D(ticksDir);
  normalize3D(ticksDir);
  
  // convert it into pixel coordinates
  double origin[3] = {0.0, 0.0, 0.0};
  getPixelCoordinates(ticksDir, ticksDir);
  getPixelCoordinates(origin, origin);

  // ticksDir = ticksDir - origin
  // to get ticks direction in pixel coordinates
  vectSubstract3D(ticksDir, origin, ticksDir);

  

  // normalize it in the (X, Y) frame.
  normalize2d(ticksDir);

  // apply the required number of pixels

  // get the viewport used by the axis
  int axesX;
  int axesY;
  int axesWidth;
  int axesHeight;
  sciGetViewingArea(sciGetParentSubwin(m_pDrawer->getDrawedObject()), &axesX, &axesY, &axesWidth, &axesHeight);

  // compute sizes wich would apply if axes where along X or Y axis
  double xSize = axesWidth * TICKS_PIXEL_LENGTH;
  double ySize = axesHeight * TICKS_PIXEL_LENGTH;

  // compute angle between ticks direction and x axis in pixel coordinates
  double angle = acos(Abs(ticksDir[0]));
  
  // push it between 0 and 1.
  double fact = angle * 2.0 / M_PI;

  scalarMult3D(ticksDir, (1.0 - fact) * xSize + fact * ySize, ticksDir);
  
}
/*---------------------------------------------------------------------------------*/
void PlaceTicksStrategy::computeTicksLocation(const double ticksAbscissas[],
                                              double ticksXPos[],
                                              double ticksYPos[],
                                              double ticksZPos[],
                                              char * labels[],
                                              char * labelsExponents[],
                                              int nbTicksIn,
                                              int * nbTicksOut)
{
  updateDataBounds();

  // get positions in 3D
  computeTicksLocation3D(ticksAbscissas, ticksXPos, ticksYPos, ticksZPos,
                         labels, labelsExponents, nbTicksIn, nbTicksOut);

  // convert them one by one
  for (int i = 0; i < *nbTicksOut; i++)
  {
    // convert coordinates
    double curTicksPos[3] = {ticksXPos[i], ticksYPos[i], ticksZPos[i]};
    getPixelCoordinates(curTicksPos, curTicksPos);

    // copy them back
    ticksXPos[i] = curTicksPos[0];
    ticksYPos[i] = curTicksPos[1];
    ticksZPos[i] = curTicksPos[2];
  }
}
/*---------------------------------------------------------------------------------*/
void PlaceTicksStrategy::computeGridStartPoints(const double ticksAbscissas[],
                                                  const double subticksAbscissas[],
                                                  double startXPos[],
                                                  double startYPos[],
                                                  double startZPos[],
                                                  int nbTicksIn,
                                                  int nbSubticksin,
                                                  int * nbTicksOut)
{
  computeGridStartPoints3D(ticksAbscissas, subticksAbscissas,
                           startXPos, startYPos, startZPos,
                           nbTicksIn, nbSubticksin, nbTicksOut);

  // convert them one by one
  for (int i = 0; i < *nbTicksOut; i++)
  {
    // convert coordinates
    double curStartPos[3] = {startXPos[i], startYPos[i], startZPos[i]};
    getPixelCoordinates(curStartPos, curStartPos);

    // copy them back
    startXPos[i] = curStartPos[0];
    startYPos[i] = curStartPos[1];
    startZPos[i] = curStartPos[2];
  }
}
/*---------------------------------------------------------------------------------*/
void PlaceTicksStrategy::computeGridMiddlePoints(const double ticksAbscissas[],
                                                 const double subticksAbscissas[],
                                                 double middleXPos[],
                                                 double middleYPos[],
                                                 double middleZPos[],
                                                 int nbTicksIn,
                                                 int nbSubticksin,
                                                 int * nbTicksOut)
{
  computeGridMiddlePoints3D(ticksAbscissas, subticksAbscissas,
                            middleXPos, middleYPos, middleZPos,
                            nbTicksIn, nbSubticksin, nbTicksOut);

  // convert them one by one
  for (int i = 0; i < *nbTicksOut; i++)
  {
    // convert coordinates
    double curMiddlePos[3] = {middleXPos[i], middleYPos[i], middleZPos[i]};
    getPixelCoordinates(curMiddlePos, curMiddlePos);

    // copy them back
    middleXPos[i] = curMiddlePos[0];
    middleYPos[i] = curMiddlePos[1];
    middleZPos[i] = curMiddlePos[2];
  }
}
/*---------------------------------------------------------------------------------*/
void PlaceTicksStrategy::computeGridEndPoints(const double ticksAbscissas[],
                                              const double subticksAbscissas[],
                                              double endXPos[],
                                              double endYPos[],
                                              double endZPos[],
                                              int nbTicksIn,
                                              int nbSubticksin,
                                              int * nbTicksOut)
{
  computeGridEndPoints3D(ticksAbscissas, subticksAbscissas,
                         endXPos, endYPos, endZPos,
                         nbTicksIn, nbSubticksin, nbTicksOut);

  // convert them one by one
  for (int i = 0; i < *nbTicksOut; i++)
  {
    // convert coordinates
    double curEndPos[3] = {endXPos[i], endYPos[i], endZPos[i]};
    getPixelCoordinates(curEndPos, curEndPos);

    // copy them back
    endXPos[i] = curEndPos[0];
    endYPos[i] = curEndPos[1];
    endZPos[i] = curEndPos[2];
  }
}
/*---------------------------------------------------------------------------------*/
double PlaceTicksStrategy::getOtherXBound(double xBound)
{
  if (Abs(xBound - getXmin()) < Abs(xBound - getXmax()))
  {
    // closer to xMin
    return getXmax();
  }
  else
  {
    return getXmin();
  }
}
/*---------------------------------------------------------------------------------*/
double PlaceTicksStrategy::getOtherYBound(double yBound)
{
  if (Abs(yBound - getYmin()) < Abs(yBound - getYmax()))
  {
    // closer to xMin
    return getYmax();
  }
  else
  {
    return getYmin();
  }
}
/*---------------------------------------------------------------------------------*/
double PlaceTicksStrategy::getOtherZBound(double zBound)
{
  if (Abs(zBound - getZmin()) < Abs(zBound - getZmax()))
  {
    // closer to xMin
    return getZmax();
  }
  else
  {
    return getZmin();
  }
}
/*---------------------------------------------------------------------------------*/
double PlaceTicksStrategy::findFrontXCoordinate(double zCoordinate)
{
  // same processus as for Z coordinate
  double pointXmin[3] = {getXmin(), getYmin(), zCoordinate};
  double pointXmax[3] = {getXmax(), getYmin(), zCoordinate};

  // find the one which is upper in term of pixels
  getPixelCoordinates(pointXmin, pointXmin);
  getPixelCoordinates(pointXmax, pointXmax);

  // get the lowest one
  if (pointXmax[1] > pointXmin[1])
  {
    return getXmin();
  }
  else
  {
    return getXmax();
  }
}
/*---------------------------------------------------------------------------------*/
double PlaceTicksStrategy::findBackXCoordinate(double zCoordinate)
{
  return getOtherXBound(findFrontXCoordinate(zCoordinate));
}
/*---------------------------------------------------------------------------------*/
double PlaceTicksStrategy::findLowerZCoordinate(void)
{
  // get two point with different Z.
  double pointZmin[3] = {getXmin(), getYmin(), getZmin()};
  double pointZmax[3] = {getXmin(), getYmin(), getZmax()};

  // find the one which is upper in term of pixels
  getPixelCoordinates(pointZmin, pointZmin);
  getPixelCoordinates(pointZmax, pointZmax);

  if (pointZmax[1] > pointZmin[1])
  {
    // standard case Zmax plane is higher than Zmin plane
    return getZmin();
  }
  else
  {
    return getZmax();
  }
}
/*---------------------------------------------------------------------------------*/
double PlaceTicksStrategy::findUpperZCoordinate(void)
{
  return getOtherZBound(findLowerZCoordinate());
}
/*---------------------------------------------------------------------------------*/
double PlaceTicksStrategy::findFrontYCoordinate(double zCoordinate)
{
  // same processus as for Z coordinate
  double pointYmin[3] = {getXmin(), getYmin(), zCoordinate};
  double pointYmax[3] = {getXmin(), getYmax(), zCoordinate};

  // find the one which is upper in term of pixels
  getPixelCoordinates(pointYmin, pointYmin);
  getPixelCoordinates(pointYmax, pointYmax);

  // get the lowest one
  if (pointYmax[1] > pointYmin[1])
  {
    return getYmin();
  }
  else 
  {
    return getYmax();
  }
}
/*---------------------------------------------------------------------------------*/
double PlaceTicksStrategy::findBackYCoordinate(double zCoordinate)
{
  return getOtherYBound(findFrontYCoordinate(zCoordinate));
}
/*---------------------------------------------------------------------------------*/
double PlaceTicksStrategy::findLeftMostYCoordinate(void)
{
  // same processus as for Z coordinate
  // same processus as for Z coordinate
  double pointYmin[3] = {getXmin(), getYmin(), getZmin()};
  double pointYmax[3] = {getXmin(), getYmax(), getZmin()};

  // find the one which is upper in term of pixels
  getPixelCoordinates(pointYmin, pointYmin);
  getPixelCoordinates(pointYmax, pointYmax);

  // get the one which is most on the left
  if (pointYmax[0] > pointYmin[0])
  {
    return getYmin();
  }
  else
  {
    return getYmax();
  }
}
/*---------------------------------------------------------------------------------*/
double PlaceTicksStrategy::findLeftMostXCoordinate(void)
{
  // same processus as for Z coordinate
  double pointXmin[3] = {getXmin(), getYmin(), getZmin()};
  double pointXmax[3] = {getXmax(), getYmin(), getZmin()};

  // find the one which is upper in term of pixels
  getPixelCoordinates(pointXmin, pointXmin);
  getPixelCoordinates(pointXmax, pointXmax);

  // get the one which is most on the right
  if (pointXmax[0] > pointXmin[0])
  {
    return getXmin();
  }
  else 
  {
    return getXmax();
  }
}
/*---------------------------------------------------------------------------------*/
bool PlaceTicksStrategy::checkTicksDirection(const double ticksDirection[3],
                                             const double axisSegmentStart[3],
                                             const double axisSegmentEnd[3])
{

  // compute ticks direction in pixels
  double origin[3] = {0.0, 0.0, 0.0};
  double ticksDirPix[3];
  getPixelCoordinates(ticksDirection, ticksDirPix);
  getPixelCoordinates(origin, origin);
  vectSubstract3D(ticksDirPix, origin, ticksDirPix);
  normalize3D(ticksDirPix);

  // compute axis direction in pixels
  double axisStartPix[3];
  double axisEndPix[3];
  double axisDirPix[3];
  getPixelCoordinates(axisSegmentStart, axisStartPix);
  getPixelCoordinates(axisSegmentEnd, axisEndPix);
  vectSubstract3D(axisEndPix, axisStartPix, axisDirPix);
  normalize3D(axisDirPix);

  if (Abs(DOT_PROD_3D(axisDirPix, ticksDirPix)) > MAX_COS)
  {
    // too close
    return false;
  }

  return true;
}
/*---------------------------------------------------------------------------------*/
void PlaceTicksStrategy::getZDirection(double ticksDirection[3], double zCoordinate)
{
  // new direction is along Z axis
  ticksDirection[0] = 0.0;
  ticksDirection[1] = 0.0;
  if (Abs(zCoordinate - getZmin()) < Abs(zCoordinate - getZmax())) {
    // yCoordinate is closer to Ymin
    ticksDirection[2] = getZmin() - getZmax();
  } else {
    ticksDirection[2] = getZmax() - getZmin();
  }
  normalize3D(ticksDirection);
}
/*---------------------------------------------------------------------------------*/
void PlaceTicksStrategy::getPixelCoordinates(const double userCoords[3], double pixCoords[3])
{
  double tmpPixCoords[3]; // in case userCoords == pixCoords
  getSubwinDrawer(sciGetParentSubwin(m_pDrawer->getDrawedObject()))->getCamera()->project(userCoords, tmpPixCoords);
  pixCoords[0] = tmpPixCoords[0];
  pixCoords[1] = tmpPixCoords[1];
  pixCoords[2] = tmpPixCoords[2];
}
/*---------------------------------------------------------------------------------*/
void PlaceTicksStrategy::updateDataBounds(void)
{
  // get real data bounds;
  double bounds[6];
  sciGetRealDataBounds(sciGetParentSubwin(m_pDrawer->getDrawedObject()), bounds);

  m_dXmin = bounds[0];
  m_dXmax = bounds[1];
  m_dYmin = bounds[2];
  m_dYmax = bounds[3];
  m_dZmin = bounds[4];
  m_dZmax = bounds[5];
}
/*---------------------------------------------------------------------------------*/
void PlaceTicksStrategy::setPosition(double ticksXPos[], double ticksYPos[], double ticksZPos[],
                                     double xCoordinate, double yCoordinate, double zCoordinate,
                                     char * labels[], char * labelsExponents[], int newTicksIndex, int oldTicksIndex)
{
  ticksXPos[newTicksIndex] = xCoordinate;
  ticksYPos[newTicksIndex] = yCoordinate;
  ticksZPos[newTicksIndex] = zCoordinate;

  // remove label if needed
  if (labels != NULL)
  {
    // permut labels[*nbTicksOut] and labels[i]
    // so labels[*nbTicksOut] become label[i]
    char * curLabel = labels[newTicksIndex];
    labels[newTicksIndex] = labels[oldTicksIndex];
    labels[oldTicksIndex] = curLabel;
  }

  if (labelsExponents != NULL)
  {
    char * curExponent = labelsExponents[newTicksIndex];
    labelsExponents[newTicksIndex] = labelsExponents[oldTicksIndex];
    labelsExponents[oldTicksIndex] = curExponent;
  }
}
/*---------------------------------------------------------------------------------*/
}

#undef MAX_COS
#undef TICKS_PIXEL_LENGTH
