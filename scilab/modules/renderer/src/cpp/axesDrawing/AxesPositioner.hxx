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

#ifndef _AXES_POSITIONER_HXX_
#define _AXES_POSITIONER_HXX_

#include "../subwinDrawing/AxisPositioner.hxx"
#include "DrawableAxes.h"

namespace sciGraphics
{

/**
 * Interface for determining on which edge of axes box an axe will be
 */
class AxesPositioner : public AxisPositioner
{
public:

  AxesPositioner(DrawableAxes * axes);

  virtual ~AxesPositioner(void);

  /**
   * Get the two bounds of this axis.
   */
  virtual void getAxisBounds(double startBound[3], double endBound[3]);

  /**
   * Compute the direction and length of ticks
   */
  virtual void getTicksDirection(double ticksDir[3]);

  /**
   * Convert ticks abscissas, into relative coordinates
   * between the two axis bounds
   * @param ticksPos abscissas of ticks along their axis as input
   *                 in output, get the relative coordinates between the axes bounds.
   *                 0 stands fot the start bound, 1 for the last one.
   * @param nbTicks number of ticks abascissas as input
   * @return number of ticks as output
   */
  virtual int getRelativeTicksPosition(double ticksPos[], int nbTicks);

protected:

  /*----------------------------------------------------------------------*/
  /**
   * To know if the axes is aligned with X axis or with Y axis
   */
  bool isXAxisAligned(void);
  /*----------------------------------------------------------------------*/
  DrawableAxes * m_pAxes;
  /*----------------------------------------------------------------------*/


};

}

#endif
