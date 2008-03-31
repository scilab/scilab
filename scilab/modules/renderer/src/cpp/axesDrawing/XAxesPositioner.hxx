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

#ifndef _X_AXES_POSITIONER_HXX_
#define _X_AXES_POSITIONER_HXX_

#include "../subwinDrawing/PlaceTicksStrategy.hxx"
#include "DrawableAxes.h"

namespace sciGraphics
{

/**
 * Class specialized in positioning ticks along axis aligned with
 * X coordinates
 */
class XAxesPositioner : public PlaceTicksStrategy
{
public:

  XAxesPositioner(DrawableAxes * axes);

  virtual ~XAxesPositioner(void);

  /**
   * Specify wether the axis segment should be draw or not
   * true by default, but redefined here
   */
  virtual bool isAxisSegmentDrawn(void);

protected:

  /**
   * Compute the two edges of axis segment in 3D coordinates
   */
  virtual void computeAxisBounds3D(const double ticksAbscissas[], int nbTicks, double axisStart[3], double axisEnd[3]);

  /**
   * Compute direction to be used by ticks in 3D coordinates
   */
  virtual void computeTicksDir3D(double ticksDir[3]);

  /**
   * Convert ticks abscissas along axis into positions in 3D coordinates
   * Also remove out of bounds legends and ticks.
   * @param nbTicksIn number of ticks position in ticksAbscissas
   * @param[out] nbTicksOut number of ticks in final ticks positions
   */
  virtual void computeTicksLocation3D(const double ticksAbscissas[],
                                      double ticksXPos[],
                                      double ticksYPos[],
                                      double ticksZPos[],
                                      char * labels[],
                                      char * labelsExponents[],
                                      int nbTicksIn,
                                      int * nbTicksOut);


};

}


#endif /* _X_AXES_POSITIONER_HXX_ */
