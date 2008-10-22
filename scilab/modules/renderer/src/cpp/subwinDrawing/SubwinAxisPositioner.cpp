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

#include <iostream>

#include "SubwinAxisPositioner.hxx"

extern "C"
{
#include "math_graphics.h"
#include "GetProperty.h"
}

namespace sciGraphics
{

#define MAX_COS 0.98

/*------------------------------------------------------------------------------------------*/
SubwinAxisPositioner::SubwinAxisPositioner(DrawableSubwin * subwin)
 : AxisPositioner()
{
  m_pSubwin = subwin;

  updateBounds();
}
/*------------------------------------------------------------------------------------------*/
SubwinAxisPositioner::~SubwinAxisPositioner(void)
{
  m_pSubwin = NULL;
}
/*------------------------------------------------------------------------------------------*/
double SubwinAxisPositioner::findLowerZCoordinate(void)
{
  // get two point with different Z.
  double pointZmin[3] = {m_dXmin, m_dYmin, m_dZmin};
  double pointZmax[3] = {m_dXmin, m_dYmin, m_dZmax};
		
  // find the one which is upper in term of pixels
  Camera * camera = m_pSubwin->getCamera();
		
  double zMinPix[3];
  double zMaxPix[3];
  camera->getPixelCoordinatesRaw(pointZmin, zMinPix);
  camera->getPixelCoordinatesRaw(pointZmax, zMaxPix);
		
  if (zMaxPix[1] > zMinPix[1])
  {
    // standard case Zmax plane is higher than Zmin plane
    return m_dZmax;
  }
  else
  {
    return m_dZmin;
  }
}
/*------------------------------------------------------------------------------------------*/
double SubwinAxisPositioner::findUpperZCoordinate(void)
{
  return findOtherZBound(findLowerZCoordinate());
}
/*------------------------------------------------------------------------------------------*/
void SubwinAxisPositioner::getTicksDirection(double ticksDir[3])
{
  // retrieve axis bounds
  double axisStart[3];
  double axisEnd[3];
  getAxisBounds(axisStart, axisEnd);

  // get default direction
  getDefaultTicksDirection(ticksDir, axisStart, axisEnd);


  // check it
  if (!checkTicksDirection(ticksDir, axisStart, axisEnd))
  {
    // use corrected one
    getCorrectedTicksDirection(ticksDir, axisStart, axisEnd);
  }

  // set right length
  setTicksDirectionLength(ticksDir, m_pSubwin->getDrawedObject());
}
/*------------------------------------------------------------------------------------------*/
double SubwinAxisPositioner::findOtherXBound(double xBound)
{
  if (Abs(xBound - m_dXmin) < Abs(xBound - m_dXmax))
  {
    // closer to xMin
    return m_dXmax;
  }
  else
  {
    return m_dXmin;
  }
}
/*------------------------------------------------------------------------------------------*/
double SubwinAxisPositioner::findOtherYBound(double yBound)
{
  if (Abs(yBound - m_dYmin) < Abs(yBound - m_dYmax))
  {
    // closer to yMin
    return m_dYmax;
  }
  else
  {
    return m_dYmin;
  }
}
/*------------------------------------------------------------------------------------------*/
double SubwinAxisPositioner::findOtherZBound(double zBound)
{
  if (Abs(zBound - m_dZmin) < Abs(zBound - m_dZmax))
  {
    // closer to xMin
    return m_dZmax;
  }
  else
  {
    return m_dZmin;
  }
}
/*------------------------------------------------------------------------------------------*/
double SubwinAxisPositioner::findFrontXCoordinate(double zCoordinate)
{
  double pointXmin[3] = {m_dXmin, m_dYmin, zCoordinate};
  double pointXmax[3] = {m_dXmax, m_dYmin, zCoordinate};
		
  // find the one which is upper in term of pixels
  double xMinPix[3];
  double xMaxPix[3];
  Camera * camera = m_pSubwin->getCamera();

  camera->getPixelCoordinatesRaw(pointXmin, xMinPix);
  camera->getPixelCoordinatesRaw(pointXmax, xMaxPix);
		
  // get the lowest one
  if (xMaxPix[1] > xMinPix[1]) {
    return m_dXmax;
  }
  else
  {
    return m_dXmin;
  }
}
/*------------------------------------------------------------------------------------------*/
double SubwinAxisPositioner::findFrontYCoordinate(double zCoordinate)
{
  double pointYmin[3] = {m_dXmin, m_dYmin, zCoordinate};
  double pointYmax[3] = {m_dXmin, m_dYmax, zCoordinate};
		
  // find the one which is upper in term of pixels
  double yMinPix[3];
  double yMaxPix[3];
  Camera * camera = m_pSubwin->getCamera();

  camera->getPixelCoordinatesRaw(pointYmin, yMinPix);
  camera->getPixelCoordinatesRaw(pointYmax, yMaxPix);
		
  // get the lowest one, ie the one with highest Y coordinate
  if (yMaxPix[1] > yMinPix[1]) {
    return m_dYmax;
  }
  else
  {
    return m_dYmin;
  }
}
/*------------------------------------------------------------------------------------------*/
double SubwinAxisPositioner::findBackYCoordinate(double zCoordinate)
{
  return findOtherYBound(findFrontYCoordinate(zCoordinate));
}
/*------------------------------------------------------------------------------------------*/
double SubwinAxisPositioner::findBackXCoordinate(double zCoordinate)
{
  return findOtherXBound(findFrontXCoordinate(zCoordinate));
}
/*------------------------------------------------------------------------------------------*/
bool SubwinAxisPositioner::checkTicksDirection(const double ticksDir[3],
                                               const double axisSegmentStart[3],
                                               const double axisSegmentEnd[3])
{

  Camera * camera = m_pSubwin->getCamera();
  // get origin in pixels
  double origin[3] = {0.0, 0.0, 0.0};
  camera->getPixelCoordinatesRaw(origin, origin);


  // compute ticks direction in pixels and normalized
  double ticksPix[3];
  camera->getPixelCoordinatesRaw(ticksDir, ticksPix);
  vectSubstract3D(ticksPix, origin, ticksPix);
  normalize3D(ticksPix);

  // compute axis direction in pixels
  double axisStartPix[3];
  double axisEndPix[3];
  camera->getPixelCoordinatesRaw(axisSegmentStart, axisStartPix);
  camera->getPixelCoordinatesRaw(axisSegmentEnd, axisEndPix);
  double axisDirPix[3];
  vectSubstract3D(axisEndPix, axisStartPix, axisDirPix);
  normalize3D(axisDirPix);

  // check the angle
  if (Abs(DOT_PROD_3D(axisDirPix, ticksPix)) > MAX_COS)
  {
    return false;
  }
		
  return true;

}
/*------------------------------------------------------------------------------------------*/
void SubwinAxisPositioner::updateBounds(void)
{
  double bounds[6];
  sciGetRealDataBounds(m_pSubwin->getDrawedObject(), bounds);

  m_dXmin = bounds[0];
  m_dXmax = bounds[1];
  m_dYmin = bounds[2];
  m_dYmax = bounds[3];
  m_dZmin = bounds[4];
  m_dZmax = bounds[5];
}
/*------------------------------------------------------------------------------------------*/
}
