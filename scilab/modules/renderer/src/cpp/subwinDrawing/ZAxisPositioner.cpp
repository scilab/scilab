/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Class specialized in drawing ticks  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ZAxisPositioner.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
ZAxisPositioner::ZAxisPositioner(DrawableSubwin * subwin)
  : SubwinAxisPositioner(subwin)
{
  
}
/*------------------------------------------------------------------------------------------*/
ZAxisPositioner::~ZAxisPositioner(void)
{
  
}
/*------------------------------------------------------------------------------------------*/
void ZAxisPositioner::getGridEdges(double startBound1[3], double startBound2[3],
                                   double middleBound1[3], double middleBound2[3],
                                   double endBound1[3], double endBound2[3])
{
  double xCoordinateBase = findLeftMostXCoordinate();
  double yCoordinateBase = findLeftMostYCoordinate();

  double xCoordinate = xCoordinateBase;
  double yCoordinate = yCoordinateBase;

  // stating points
  startBound1[0] = xCoordinate;
  startBound1[1] = yCoordinate;
  startBound1[2] = m_dZmin;

  startBound2[0] = xCoordinate;
  startBound2[1] = yCoordinate;
  startBound2[2] = m_dZmax;

  // middle points, invert Y bounds
	bool xAndzAxesAdjacent = isSharingEndWithXaxis(findLowerZCoordinate(), xCoordinate);
	bool drawFront = (sciGetGridFront(m_pSubwin->getDrawedObject()) == TRUE);
  if (   (xAndzAxesAdjacent && !drawFront)
		  || (!xAndzAxesAdjacent && drawFront))
  {
    // invert y
    yCoordinate = findOtherYBound(yCoordinateBase);
  }
  else
  {
    // invert x
    xCoordinate = findOtherXBound(xCoordinateBase);
  }

  middleBound1[0] = xCoordinate;
  middleBound1[1] = yCoordinate;
  middleBound1[2] = m_dZmin;

  middleBound2[0] = xCoordinate;
  middleBound2[1] = yCoordinate;
  middleBound2[2] = m_dZmax;

  // end points, invert X and Y
  xCoordinate = findOtherXBound(xCoordinateBase);
  yCoordinate = findOtherYBound(yCoordinateBase);
  endBound1[0] = xCoordinate;
  endBound1[1] = yCoordinate;
  endBound1[2] = m_dZmin;

  endBound2[0] = xCoordinate;
  endBound2[1] = yCoordinate;
  endBound2[2] = m_dZmax;
}
/*------------------------------------------------------------------------------------------*/
void ZAxisPositioner::getAxisBounds(double startBound[3], double endBound[3])
{
  double xCoordinate = findLeftMostXCoordinate();
  double yCoordinate = findLeftMostYCoordinate();

  startBound[0] = xCoordinate;
  startBound[1] = yCoordinate;
  startBound[2] = m_dZmin;

  endBound[0] = xCoordinate;
  endBound[1] = yCoordinate;
  endBound[2] = m_dZmax;
}
/*------------------------------------------------------------------------------------------*/
int ZAxisPositioner::getRelativeTicksPosition(double ticksPos[], int nbTicks)
{

  for (int i = 0; i < nbTicks; i++)
  {
    ticksPos[i] = (ticksPos[i] - m_dZmin) / (m_dZmax - m_dZmin);
  }
  return nbTicks;
}
/*------------------------------------------------------------------------------------------*/
double ZAxisPositioner::findLeftMostXCoordinate(void)
{	
  double pointXmin[3] = {m_dXmin, m_dYmin, m_dZmin};
  double pointXmax[3] = {m_dXmax, m_dYmin, m_dZmin};
		
  // find the one which is upper in term of pixels
  double xMinPix[3];
  double xMaxPix[3];
  Camera * camera = m_pSubwin->getCamera();

  camera->getPixelCoordinatesRaw(pointXmin, xMinPix);
  camera->getPixelCoordinatesRaw(pointXmax, xMaxPix);
		
  // get the lowest one
  if (xMaxPix[0] > xMinPix[0]) {
    return m_dXmin;
  }
  else
  {
    return m_dXmax;
  }
}
/*------------------------------------------------------------------------------------------*/
double ZAxisPositioner::findLeftMostYCoordinate(void)
{
  double pointYmin[3] = {m_dXmin, m_dYmin, m_dZmin};
  double pointYmax[3] = {m_dXmin, m_dYmax, m_dZmin};
		
  // find the one which is upper in term of pixels
  double yMinPix[3];
  double yMaxPix[3];
  Camera * camera = m_pSubwin->getCamera();

  camera->getPixelCoordinatesRaw(pointYmin, yMinPix);
  camera->getPixelCoordinatesRaw(pointYmax, yMaxPix);
		
  // get the lowest one
  if (yMaxPix[0] > yMinPix[0]) {
    return m_dYmin;
  }
  else
  {
    return m_dYmax;
  }
}
/*------------------------------------------------------------------------------------------*/
bool ZAxisPositioner::isSharingEndWithXaxis(double zCoordFront, double xCoordinate)
{
  // get the three coordinates of the edge whuch is common for X and Y axis in default mode
  double xCoordFront = findFrontXCoordinate(zCoordFront);
  double xCoordBack  = findOtherXBound(xCoordFront);
		
  // equivalent to return xCoordinate == xCoordBack
  return (Abs(xCoordinate - xCoordBack) < Abs(xCoordinate - xCoordFront));
}
/*------------------------------------------------------------------------------------------*/
void ZAxisPositioner::getDefaultTicksDirection(double ticksDir[3],
                                               const double axisSegmentStart[3],
                                               const double axisSegmentEnd[3])
{
  // we must find wether Z axis ticks are directed by X axis or Y axis
  // Actually Z axis is directed by the axis with which it shares on edge
  // when X and Y axis are in defautl mode, ie bottom and left
	
  // get the three coordinates of the edge which is common for X and Y axis in default mode
  double zCoordFront = findLowerZCoordinate();
  double xCoordinate = axisSegmentStart[0];
  double yCoordinate = axisSegmentStart[1];

  if (isSharingEndWithXaxis(zCoordFront, xCoordinate))
  {
    ticksDir[0] = xCoordinate - findFrontXCoordinate(zCoordFront);
    ticksDir[1] = 0.0;
    ticksDir[2] = 0.0;
  }
  else
  {
    ticksDir[0] = 0.0;
    ticksDir[1] = yCoordinate - findFrontYCoordinate(zCoordFront);
    ticksDir[2] = 0.0;
  }
}
/*------------------------------------------------------------------------------------------*/
void ZAxisPositioner::getCorrectedTicksDirection(double ticksDir[3],
                                                 const double axisSegmentStart[3],
                                                 const double axisSegmentEnd[3])
{
  double zCoordFront = findLowerZCoordinate();
  double xCoordinate = axisSegmentStart[0];
  double yCoordinate = axisSegmentStart[1];

  if (!isSharingEndWithXaxis(zCoordFront, xCoordinate))
  {
    ticksDir[0] = xCoordinate - findBackXCoordinate(zCoordFront);
    ticksDir[1] = 0.0;
    ticksDir[2] = 0.0;
  }
  else
  {
    ticksDir[0] = 0.0;
    ticksDir[1] = yCoordinate - findBackYCoordinate(zCoordFront);
    ticksDir[2] = 0.0;
  }
}
/*------------------------------------------------------------------------------------------*/

}
