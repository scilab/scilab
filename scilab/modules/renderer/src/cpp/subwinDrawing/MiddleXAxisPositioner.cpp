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

#include "MiddleXAxisPositioner.hxx"

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
MiddleXAxisPositioner::MiddleXAxisPositioner(DrawableSubwin * subwin)
  : XAxisPositioner(subwin)
{

}
/*------------------------------------------------------------------------------------------*/
MiddleXAxisPositioner::~MiddleXAxisPositioner(void)
{

}
/*------------------------------------------------------------------------------------------*/
void MiddleXAxisPositioner::getAxisBounds(double startBound[3], double endBound[3])
{
  double zCoordinate = findUpperZCoordinate();
  double yCoordinate = (m_dYmin + m_dYmax) / 2.0;

  startBound[0] = m_dXmin;
  startBound[1] = yCoordinate;
  startBound[2] = zCoordinate;

  endBound[0] = m_dXmax;
  endBound[1] = yCoordinate;
  endBound[2] = zCoordinate;
}
/*------------------------------------------------------------------------------------------*/

}
