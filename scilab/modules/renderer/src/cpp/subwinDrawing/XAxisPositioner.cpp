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

#include "XAxisPositioner.hxx"

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
XAxisPositioner::XAxisPositioner(DrawableSubwin * subwin)
  : SubwinAxisPositioner(subwin)
{
  
}
/*------------------------------------------------------------------------------------------*/
XAxisPositioner::~XAxisPositioner(void)
{
  
}
/*------------------------------------------------------------------------------------------*/
void XAxisPositioner::getGridEdges(double startBound1[3], double startBound2[3],
                                   double middleBound1[3], double middleBound2[3],
                                   double endBound1[3], double endBound2[3])
{
  double zCoordinate = findLowerZCoordinate();
  double yCoordinate = findFrontYCoordinate(zCoordinate);

  // stating points
  startBound1[0] = m_dXmin;
  startBound1[1] = yCoordinate;
  startBound1[2] = zCoordinate;

  startBound2[0] = m_dXmax;
  startBound2[1] = yCoordinate;
  startBound2[2] = zCoordinate;

  // middle points, invert Y bounds
  yCoordinate = findOtherYBound(yCoordinate);
  middleBound1[0] = m_dXmin;
  middleBound1[1] = yCoordinate;
  middleBound1[2] = zCoordinate;

  middleBound2[0] = m_dXmax;
  middleBound2[1] = yCoordinate;
  middleBound2[2] = zCoordinate;

  // end points, invert Z bounds
  zCoordinate = findOtherZBound(zCoordinate);
  endBound1[0] = m_dXmin;
  endBound1[1] = yCoordinate;
  endBound1[2] = zCoordinate;

  endBound2[0] = m_dXmax;
  endBound2[1] = yCoordinate;
  endBound2[2] = zCoordinate;
}
/*------------------------------------------------------------------------------------------*/
int XAxisPositioner::getRelativeTicksPosition(double ticksPos[], int nbTicks)
{
  for (int i = 0; i < nbTicks; i++)
  {
    ticksPos[i] = (ticksPos[i] - m_dXmin) / (m_dXmax - m_dXmin);
  }
  return nbTicks;
}
/*------------------------------------------------------------------------------------------*/
void XAxisPositioner::getDefaultTicksDirection(double ticksDir[3],
                                               const double axisSegmentStart[3],
                                               const double axisSegmentEnd[3])
{
  double yCoordinate = axisSegmentStart[1];
  if (Abs(yCoordinate - m_dYmin) <= Abs(yCoordinate - m_dYmax))
  {
    // yCoordinate is closer to Ymin
    ticksDir[0] = 0.0;
    ticksDir[1] = m_dYmin - m_dYmax;
    ticksDir[2] = 0.0;
  }
  else
  {
    ticksDir[0] = 0.0;
    ticksDir[1] = m_dYmax - m_dYmin;
    ticksDir[2] = 0.0;
  }
}
/*------------------------------------------------------------------------------------------*/
void XAxisPositioner::getCorrectedTicksDirection(double ticksDir[3],
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
