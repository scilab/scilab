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

#ifndef _AXIS_POSITIONER_HXX_
#define _AXIS_POSITIONER_HXX_

extern "C"
{
#include "ObjectStructure.h"
}

namespace sciGraphics
{

/**
 * Interface for determining on which edge of axes box an axe will be
 */
class AxisPositioner
{
public:

  AxisPositioner(void) {}

  virtual ~AxisPositioner(void) {}

  /**
   * Get the two bounds of this axis.
   */
  virtual void getAxisBounds(double startBound[3], double endBound[3]) = 0;

  /**
   * Find the edges on which the grid will be displayed from the axes.
   * Box. Grid is drawn on 2 facets, then 3 edges are necessary.
   * This function is not useful for all axes drawer and drawer
   * which need it must redefine it.
	 * @param drawFront specify whether the grid should be drawn above other
	 *                  objects or behind
   */
  virtual void getGridEdges(double startBound1[3], double startBound2[3],
                            double middleBound1[3], double middleBound2[3],
                            double endBound1[3], double endBound2[3]) {}

  /**
   * Compute the direction and length of ticks
   */
  virtual void getTicksDirection(double ticksDir[3]) = 0;

  /**
   * Convert ticks abscissas, into relative coordinates
   * between the two axis bounds
   * @param ticksPos abscissas of ticks along their axis as input
   *                 in output, get the relative coordinates between the axes bounds.
   *                 0 stands fot the start bound, 1 for the last one.
   * @param nbTicks number of ticks abascissas as input
   * @return number of ticks as output
   */
  virtual int getRelativeTicksPosition(double ticksPos[], int nbTicks) = 0;

protected:

  /**
   * Modify the length of ticks dir depending on the size of the parent subwin.
   */
  virtual void setTicksDirectionLength(double ticksDir[3], sciPointObj * pSubwin);

};

}

#endif
