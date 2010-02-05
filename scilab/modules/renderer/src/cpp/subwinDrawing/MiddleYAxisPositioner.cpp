/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * desc : Class specialized in drawing ticks  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "MiddleYAxisPositioner.hxx"

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
MiddleYAxisPositioner::MiddleYAxisPositioner(DrawableSubwin * subwin)
  : YAxisPositioner(subwin)
{

}
/*------------------------------------------------------------------------------------------*/
MiddleYAxisPositioner::~MiddleYAxisPositioner(void)
{

}
/*------------------------------------------------------------------------------------------*/
void MiddleYAxisPositioner::getAxisBounds(double startBound[3], double endBound[3])
{
  double zCoordinate = findUpperZCoordinate();
  double xCoordinate = (m_dXmin + m_dXmax) / 2.0;

  startBound[0] = xCoordinate;
  startBound[1] = m_dYmin;
  startBound[2] = zCoordinate;

  endBound[0] = xCoordinate;
  endBound[1] = m_dYmax;
  endBound[2] = zCoordinate;
}
/*------------------------------------------------------------------------------------------*/
}
