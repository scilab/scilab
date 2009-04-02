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

#include "YAxisPositioner.hxx"

extern "C"
{
#include "GetProperty.h"
#include "math_graphics.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
YAxisPositioner::YAxisPositioner(DrawableSubwin * subwin)
  : SubwinAxisPositioner(subwin)
{
  
}
/*------------------------------------------------------------------------------------------*/
YAxisPositioner::~YAxisPositioner(void)
{
  
}
/*------------------------------------------------------------------------------------------*/
void YAxisPositioner::getGridEdges(double startBound1[3], double startBound2[3],
                                   double middleBound1[3], double middleBound2[3],
                                   double endBound1[3], double endBound2[3])
{
  double zCoordinate = findLowerZCoordinate();
  double xCoordinate = findFrontXCoordinate(zCoordinate);

  // stating points
  startBound1[0] = xCoordinate;
  startBound1[1] = m_dYmin;
  startBound1[2] = zCoordinate;

  startBound2[0] = xCoordinate;
  startBound2[1] = m_dYmax;
  startBound2[2] = zCoordinate;

  // middle points.
	if (sciGetGridFront(m_pSubwin->getDrawedObject()))
	{
		// invert Z coordinate
		zCoordinate = findOtherZBound(zCoordinate);
	}
	else
	{
		// invert X coordinate
		xCoordinate = findOtherXBound(xCoordinate);
	}
  middleBound1[0] = xCoordinate;
  middleBound1[1] = m_dYmin;
  middleBound1[2] = zCoordinate;

  middleBound2[0] = xCoordinate;
  middleBound2[1] = m_dYmax;
  middleBound2[2] = zCoordinate;

  // end points, invert the other coordinate
  if (sciGetGridFront(m_pSubwin->getDrawedObject()))
	{
		// invert X coordinate
		xCoordinate = findOtherXBound(xCoordinate);
	}
	else
	{
		// invert Z coordinate
		zCoordinate = findOtherZBound(zCoordinate);
	}
  endBound1[0] = xCoordinate;
  endBound1[1] = m_dYmin;
  endBound1[2] = zCoordinate;

  endBound2[0] = xCoordinate;
  endBound2[1] = m_dYmax;
  endBound2[2] = zCoordinate;
}
/*------------------------------------------------------------------------------------------*/
int YAxisPositioner::getRelativeTicksPosition(double ticksPos[], int nbTicks)
{

  for (int i = 0; i < nbTicks; i++)
  {
    ticksPos[i] = (ticksPos[i] - m_dYmin) / (m_dYmax - m_dYmin);
  }
  return nbTicks;
}
/*------------------------------------------------------------------------------------------*/
void YAxisPositioner::getDefaultTicksDirection(double ticksDir[3],
                                               const double axisSegmentStart[3],
                                               const double axisSegmentEnd[3])
{
  double xCoordinate = axisSegmentStart[0];
  if (Abs(xCoordinate - m_dXmin) <= Abs(xCoordinate - m_dXmax))
  {
    // xCoordinate is closer to Xmin
    ticksDir[0] = m_dXmin - m_dXmax;
    ticksDir[1] = 0.0;
    ticksDir[2] = 0.0;
  }
  else
  {
    ticksDir[0] = m_dXmax - m_dXmin;
    ticksDir[1] = 0.0;
    ticksDir[2] = 0.0;
  }
}
/*------------------------------------------------------------------------------------------*/
void YAxisPositioner::getCorrectedTicksDirection(double ticksDir[3],
                                                 const double axisSegmentStart[3],
                                                 const double axisSegmentEnd[3])
{
  // use Z axis instead
  double zCoordinate = axisSegmentStart[2];
  if (Abs(zCoordinate - m_dZmin) < Abs(zCoordinate - m_dZmax))
  {
    // yCoordinate is closer to Ymin
    ticksDir[0] = 0.0;
    ticksDir[1] = 0.0;
    ticksDir[2] = m_dZmin - m_dZmax;
  }
  else
  {
    ticksDir[0] = 0.0;
    ticksDir[1] = 0.0;
    ticksDir[2] = m_dZmax - m_dZmin;
  }
}
/*------------------------------------------------------------------------------------------*/

}
