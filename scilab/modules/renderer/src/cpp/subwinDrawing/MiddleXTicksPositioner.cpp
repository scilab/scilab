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

#include "MiddleXTicksPositioner.hxx"

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
MiddleXTicksPositioner::MiddleXTicksPositioner(DrawableSubwin * subwin)
  : XTicksPositioner(subwin)
{

}
/*---------------------------------------------------------------------------------*/
MiddleXTicksPositioner::~MiddleXTicksPositioner(void)
{

}
/*---------------------------------------------------------------------------------*/
double MiddleXTicksPositioner::findZCoordinate(void)
{
  return findUpperZCoordinate();
}
/*---------------------------------------------------------------------------------*/
double MiddleXTicksPositioner::findYCoordinate(double zCoordinate)
{
  // find if 0 is in the range of Y coordinates
  // if so then one of yMax or yMax is <= 0 and the other is not
  if (getYmin() * getYmax() <= 0.0)
  {
    return 0.0;
  }
  else if (getYmax() < 0.0)
  {
    // both bounds are negative
    return getYmax();
  }
  else
  {
    // both bounds are positive
    return getYmin();
  }
}
/*---------------------------------------------------------------------------------*/

}
